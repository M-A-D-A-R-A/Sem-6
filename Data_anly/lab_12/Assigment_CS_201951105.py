# %% [markdown]
# # Nishant Andoriya
# # 201951105
# # Assigment 12

# %%
import pandas as pd
import random
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats

# %%
df_1=pd.read_csv("cookie_cats.csv")
df_1.head()

# %%
result = stats.ttest_ind(df_1[df_1.version == 'gate_30'].sum_gamerounds,df_1[df_1.version == 'gate_40'].sum_gamerounds,equal_var=False)


# %%
print(f'The value of t : {result.statistic:.4f}')
print(f'The value for p for single sided test : {result.pvalue / 2:.4f}')

# %% [markdown]
# # Observation & Conclusion
# - As we can observe that p-value is very higher than than the alpha (i.e. 0.05), so We should not reject the null hypothesis 


