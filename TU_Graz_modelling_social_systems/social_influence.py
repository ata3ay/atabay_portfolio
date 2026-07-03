import mesa
from mesa import Agent,Model
from mesa.space import SingleGrid
from mesa.datacollection import DataCollector
import matplotlib.pyplot as plt

#--------------------------------------
class AxelrodAgent(Agent):
    def __init__(self, model):
        super().__init__(model)
        self.profile = []
        for _ in range(self.model.amount_of_features):
            self.profile.append(self.random.randrange(self.model.traits_per_feature))
        self.cached_neighbors=None

#------------------------------------------------------------
    def get_neighbors(self):
        if self.cached_neighbors is None:
            self.cached_neighbors = self.model.grid.get_neighbors(self.pos, moore=False)
        return self.cached_neighbors
#----------------------------------
    def similarity(self, other):
        matches= sum( 
            1 for i in range(self.model.amount_of_features) if self.profile[i]==other.profile[i])
        return matches/self.model.amount_of_features
                     
#-----------------------------------
    def step(self):
        neighbors=self.get_neighbors()
        if not neighbors:
            return
    
        neighbor= self.random.choice(neighbors)
        if self.random.random()<self.similarity(neighbor):
            different=[i for i in range(self.model.amount_of_features) if self.profile[i]!=neighbor.profile[i]]
            if different:
                f=self.random.choice(different)
                self.profile[f]= neighbor.profile[f]

#------------------------------
#----------------------------------------
class AxelrodModel(Model):
    def __init__(self, width=10, height=10, amount_of_features=4,traits_per_feature=10,seed=None):
        super().__init__(seed=seed)

      
        self.amount_of_features = amount_of_features
        self.traits_per_feature= traits_per_feature
        self.grid = SingleGrid(width, height, torus=False)


        for x in range(width):
            for y in range(height):
                self.grid.place_agent(AxelrodAgent(self), (x, y))

        self.running = True
        self.agent_list=list(self.agents)

    def step(self):
        for _ in range(self.grid.width * self.grid.height):
            self.random.choice(self.agent_list).step()

#----GRANOVETTER
class GranovetterAgent(Agent):
    def __init__(self, model, profile):
        super().__init__(model)
        self.profile    = profile
        self.opinion_ai = self.random.choice([-1, 0, 1])
        self.threshold  = self.random.uniform(
            model.threshold_min, model.threshold_max
        )
        self.cached_neighbors = None

    def get_neighbors(self):
        if self.cached_neighbors is None:
            self.cached_neighbors = self.model.grid.get_neighbors(
                self.pos, moore=False, include_center=False
            )
        return self.cached_neighbors

    def similarity(self, other):
        matches = sum(
            1 for i in range(self.model.amount_of_features)
            if self.profile[i] == other.profile[i]
        )
        return matches / self.model.amount_of_features

    def step(self):
        neighbors = self.get_neighbors()
        if not neighbors:
            return

        total    = 0
        pro_sum  = 0
        anti_sum = 0

        for n in neighbors:
            sim   = self.similarity(n)
            total += sim
            if n.opinion_ai == 1:
                pro_sum  += sim
            elif n.opinion_ai == -1:
                anti_sum += sim

        if total == 0:
            return

        if pro_sum / total >= self.threshold:
            self.opinion_ai = 1
        elif anti_sum / total >= self.threshold:
            self.opinion_ai = -1


class GranovetterModel(Model):
    def __init__(self, axelrod_model,
                 threshold_min=0.2, threshold_max=0.8,
                 seed=None):
        super().__init__(seed=seed)
        self.threshold_min      = threshold_min
        self.threshold_max      = threshold_max
        self.amount_of_features = axelrod_model.amount_of_features

        width  = axelrod_model.grid.width
        height = axelrod_model.grid.height
        self.grid = SingleGrid(width, height, torus=False)

        for agent in axelrod_model.agents:
            x, y = agent.pos
            self.grid.place_agent(
                GranovetterAgent(self, profile=agent.profile.copy()), (x, y)
            )

        self.agent_list = list(self.agents)
        self.running    = True

        self.datacollector = DataCollector(
            model_reporters={
                "Pro-AI":   lambda m: sum(1 for a in m.agents if a.opinion_ai == 1),
                "Neutral":  lambda m: sum(1 for a in m.agents if a.opinion_ai == 0),
                "Anti-AI":  lambda m: sum(1 for a in m.agents if a.opinion_ai == -1),
            }
        )
        self.datacollector.collect(self)

    def step(self):
        for _ in range(self.grid.width * self.grid.height):
            self.random.choice(self.agent_list).step()
        self.datacollector.collect(self)

def find_tipping_point(df, opinion='Pro-AI', majority=50):

    for step, count in enumerate(df[opinion]):
        if count >= majority:
            return step
    return None #opinion is not reached

def plot_tipping_steps(axelrod_model, threshold_configs, steps=100):

    tipping_pro_ai = []
    tipping_anti_ai = []
    labels = []

    for t_min, t_max in threshold_configs:
        model = GranovetterModel(axelrod_model=axelrod_model,
                                 threshold_min=t_min,
                                 threshold_max=t_max,
                                 seed = 77)
        
        for _ in range(steps):
            model.step()

        df=model.datacollector.get_model_vars_dataframe()

        tp_pro_ai = None
        tp_anti_ai = None

        for step, row in df.iterrows():
            if row['Pro-AI'] >= 50 and tp_pro_ai is None:
                tp_pro_ai = step
            if row['Anti-AI'] >= 50 and tp_anti_ai is None:
                tp_anti_ai = step

        if tp_pro_ai is None:
            tp_pro_ai = steps + 1
        if tp_anti_ai is None:
            tp_anti_ai = steps + 1

        tipping_pro_ai.append(tp_pro_ai)
        tipping_anti_ai.append(tp_anti_ai)
        labels.append(f'[{t_min}, {t_max}]')

    x = range(len(labels))
    width = 0.35

    plt.figure(figsize=(10, 5))

    plt.bar([i - width/2 for i in x], tipping_pro_ai, width,label='Pro-AI',  color='green')
    plt.bar([i + width/2 for i in x], tipping_anti_ai,width,label='Anti-AI', color='red')

    plt.axhline(steps + 1, linestyle='--', color='black', label='Never reached majority')

    plt.title('At which step did social influence become strong enough?')
    plt.xlabel('Threshold range')
    plt.ylabel('Simulation step')
    plt.xticks(list(x), labels)
    plt.legend()
    plt.savefig('plot_Tipping_steps.png')
    plt.show()

def plot_opinion_over_time(df):

    plt.figure(figsize=(10,5))

    plt.plot(df['Pro-AI'], color='green', linewidth=2, label='Pro-AI')
    plt.plot(df['Neutral'], color='grey', linewidth=2, label='Neutral')
    plt.plot(df['Anti-AI'], color='red', linewidth=2, label='Anti-AI')

    tp = find_tipping_point(df, opinion='Pro-AI')
    if tp is not None:
        plt.axvline(tp,linestyle='--', color='green', linewidth=1.5, label=f'Tipping point {tp}')
    
    plt.title('Opinion adoption over time')
    plt.xlabel('Simulation step')
    plt.ylabel('Number of agents')

    plt.legend()
    plt.savefig('plot1_opinion.png')
    plt.show()


def plot_threshold_exp(axelrod_model, threshold_configs, steps=100):

    pro_counts     = []
    neutral_counts = []
    anti_counts    = []
    labels         = []

    for t_min, t_max in threshold_configs:
        model = GranovetterModel(axelrod_model=axelrod_model,
                                 threshold_min=t_min,
                                 threshold_max=t_max,
                                 seed=77)

        for _ in range(steps):         
            model.step()

        # gets collected data as dataframe and takes the last row as final state
        df    = model.datacollector.get_model_vars_dataframe()  
        final = df.iloc[-1]

        pro_counts.append(int(final['Pro-AI']))      
        neutral_counts.append(int(final['Neutral'])) 
        anti_counts.append(int(final['Anti-AI']))    
        labels.append(f'[{t_min}, {t_max}]')         

    x= range(len(labels))
    width = 0.25

    plt.figure(figsize=(10, 5))

    # drawing 3 groups of bars
    plt.bar([i - width for i in x], pro_counts,width, label='Pro-AI',  color='green')
    plt.bar([i for i in x], neutral_counts, width, label='Neutral', color='grey')
    plt.bar([i + width for i in x], anti_counts,width, label='Anti-AI', color='red')

    plt.title('Opinion distribution per threshold range')
    plt.xlabel('Threshold range')
    plt.ylabel('Number of agents')
    plt.xticks(list(x), labels)
    plt.legend()
    plt.savefig('plot2_threshold_exp.png')
    plt.show()




#--RUNNING THE CODE

if __name__=="__main__":

    axelrod= AxelrodModel(width=10,height=10,amount_of_features=4,traits_per_feature=10,seed=42)
    for _ in range(200):
        axelrod.step()
    granovetter = GranovetterModel(axelrod_model=axelrod,
                                   threshold_min=0.2,
                                   threshold_max=0.8,
                                   seed=42)
    for _ in range(200):
        granovetter.step()

#RESULTS
df = granovetter.datacollector.get_model_vars_dataframe()
threshold_configs = [
        (0.1, 0.3),
        (0.2, 0.5),
        (0.2, 0.8),
        (0.4, 0.8),
        (0.6, 0.9),
    ]
plot_opinion_over_time(df)
plot_threshold_exp(axelrod, threshold_configs, steps=100)
plot_tipping_steps(axelrod, threshold_configs, steps=100)


df.plot(title="Does online opinion influence individual opinions about AI?",
            xlabel="Simulation step",
            ylabel="Number of agents")
           
plt.grid()
plt.tight_layout()
plt.savefig("results.png")
plt.show()
