# %% [markdown]
# Nishant Andoriya
# 
# 201951105
# 
# Assigment 11

# %%
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import random

# %%
data = pd.read_csv("cookie_cats.csv")
data

# %% [markdown]
# Check which version of the game is better by computing average ”sum gamerounds”.

# %%
# Difference
mean_a = data[data['version'] == 'gate_30'].sum_gamerounds.mean()
mean_b = data[data['version'] == 'gate_40'].sum_gamerounds.mean()
print("Mean sum of gamerounds for gate_30 : ", mean_a)
print("Mean sum of gamerounds for gate_40 : ", mean_b)
print("Difference Between average of sum gamerounds : ", mean_a - mean_b  )

# %%
ax = data.boxplot(by='version', column='sum_gamerounds')
ax.set_xlabel('')
ax.set_ylabel('Time (in seconds)')
plt.suptitle('')

# %% [markdown]
# Perform the permutation Resampling on the data and derive the probability. Make the decision based on derived probability.

# %% [markdown]
# Permutation Test
# 
# Steps to perform permutation test,
# 
# Combine the results from the different groups into a single data set.
# 
# Shuffle the combined data and then randomly draw (without replacement) a resample of the same size as group A (clearly it will contain some data from the other groups).
# 
# From the remaining data, randomly draw (without replacement) a resample of the same size as group B.
# 
# Do the same for groups C, D, and so on. You have now collected one set of resamples that mirror the sizes of the original samples.
# 
# Whatever statistic or estimate was calculated for the original samples (e.g., difference in group proportions), calculate it now for the resamples, and record; this constitutes one permutation iteration.

# %%
# Resampling
def perm_fun(x, nA, nB):
    n = nA + nB
    idx_B = set(random.sample(range(n), nB))
    idx_A = set(range(n)) - idx_B
    return x.loc[idx_A].mean() - x.loc[idx_B].mean()

nA= len(data[data['version'] == 'gate_30'])
print(nA)
nB=len(data[data['version'] == 'gate_40'])
print(nB)
perm_diffs = [perm_fun(data.sum_gamerounds, nA, nB) for _ in range(1000)]

# %%
fig, ax = plt.subplots(figsize=(5, 5))
ax.hist(perm_diffs, bins=11, rwidth=0.9)

# original mean difference
ax.axvline(x= mean_a -mean_b, color='black', lw=2)

ax.set_xlabel('Session time differences (in seconds)')
ax.set_ylabel('Frequency')
plt.show()


# %%
np.mean(perm_diffs > mean_a - mean_b)

# %% [markdown]
# Yes. Since above percentage of total permutation is small, the difference between mean time spent on gate_30 and gate_34 is by chance.


