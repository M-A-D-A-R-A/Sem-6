
import pandas as pd
import numpy as np
from sklearn.model_selection import StratifiedShuffleSplit
import seaborn as sns

filepath="weight-height.csv"
data=pd.read_csv(filepath)

#Store the real mean in a separate variable
real_mean = round(data['Height'].mean(),3)
real_mean


# Obtain simple random sample
simple_random_sample = data.sample(n=1000).sort_index()

# Save the sample mean in a separate variable
simple_random_mean = round(simple_random_sample['Height'].mean(),3)

# View sampled data frame
simple_random_sample

def systematic_sampling(df, step):
    
    indexes = np.arange(0,len(df),step=step)
    systematic_sample = df.iloc[indexes]
    return systematic_sample
    
# Obtain a systematic sample and save it in a new variable
systematic_sample = systematic_sampling(data, 10)

# Save the sample mean in a separate variable
systematic_mean = round(systematic_sample['Height'].mean(),3)

# View sampled data frame
systematic_sample

# Set the split criteria
split = StratifiedShuffleSplit(n_splits=1, test_size=1000)
print(split)

# Perform data frame split
for x, y in split.split(data, data['Gender']):
    stratified_random_sample = data.iloc[y]

# View sampled data frame

stratified_random_sample_mean = round(stratified_random_sample['Height'].mean(),3)


# Obtain the sample mean for each group
stratified_random_sample.groupby("Gender").mean()

stratified_random_sample

# Create a dictionary with the mean outcomes for each sampling method and the real mean
outcomes = {'sample_mean':[simple_random_mean,systematic_mean,stratified_random_sample_mean],
           'population_mean':real_mean}

# Transform dictionary into a data frame
outcomes = pd.DataFrame(outcomes, index=['Simple Random Sampling','Systematic Sampling','Stratified Sampling'])

# Add a value corresponding to the absolute error
outcomes['abs_error'] = abs(outcomes['population_mean'] - outcomes['sample_mean'])

# Sort data frame by absolute error
outcomes.sort_values(by='abs_error')
