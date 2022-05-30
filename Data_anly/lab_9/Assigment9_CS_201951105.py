# %% [markdown]
# ### Sameer anand
# ### 201951134

# %%
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import math

sns.set(rc={'figure.figsize':(10,10)})

# %%
data = pd.read_csv("./weight-height.csv")
height_data = data["Height"]
height_data.head()

# %% [markdown]
# 1) Verify the Central Limit Theorem using ”Height” feature of the data.

# %%
#Store the real mean in a separate variable
real_mean = round(data['Height'].mean(),3)

# %%
# Size of samples
num = [1, 10, 50, 100]
means = []

for j in num:
    # Generate 1000 samples of size j from the height data
    x = [round(height_data.sample(n=j).mean(), 3) for i in range(1000)]
    # append the mean of all the 1000 samples to x
    means.append(x)


# %%
k = 0
fig, ax = plt.subplots(2, 2, figsize=(10, 10))
bell = np.random.normal(size=10000)

for i in range(0, 2):
    for j in range(0, 2):
        sns.histplot(data=means[k],ax=ax[i,j],kde=True,bins=20)
        ax[i, j].set_title(label="Sample Size {x}".format(x = num[k]))
        k = k + 1


# %% [markdown]
# 2) Perform the Bootstrap on ”Height” feature of the data.

# %%
number_of_samples=10000 # R times
size_of_sample=300  # n
sample_mean=[]
for i in range(number_of_samples):
    # Calulate mean for n samples
    sample_mean.append(np.mean(np.random.choice(height_data,size_of_sample,replace=True)))

# %%
_=plt.hist(sample_mean,bins=100)
standard_error=np.std(sample_mean)/math.sqrt(len(sample_mean))
print("standard error : ", standard_error)

# %% [markdown]
# 3) Calculate the Confidence Interval of 95 % using sample means derived using Bootstrap.

# %%
CI=0.95
sorted_means=np.sort(sample_mean)
l=len(sorted_means)
idx=math.floor(l*((1-CI)/2))

print("Lower level :", sorted_means[idx])
print("Upper level :", sorted_means[l-idx-1])