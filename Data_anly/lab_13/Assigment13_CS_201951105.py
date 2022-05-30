# %% [markdown]
# # Nishant Andoriya
# # 201951105
# # Assigment 12

# %%
import pandas as pd
import numpy as np
from scipy import stats
import statsmodels.api as sm
from statsmodels.formula.api import ols

# %%
location_cookie_cats_data = "cookie_cats.csv"

# %%
cookie_cats_data = pd.read_csv(location_cookie_cats_data);
cookie_cats_data

# %%
temp_data = cookie_cats_data.set_index("version")

# %%
version_data = temp_data.loc["gate_30"].reset_index()
version_data["gate_40"] = temp_data.loc["gate_40"].reset_index()["sum_gamerounds"]
version_data["gate_30"] = temp_data.loc["gate_30"].reset_index()["sum_gamerounds"]
version_data = version_data.iloc[:,5:]
version_data

# %%
# Derivation using Observed means method 
observed_variance_data = cookie_cats_data.groupby('version').mean().var()[0]
print('mean observed:', cookie_cats_data.groupby('version').mean().values.ravel())
print('variance:', observed_variance_data)

# %%
def perm_test(data):
    data = data.copy()
    data['sum_gamerounds'] = np.random.permutation(data['sum_gamerounds'].values)
    return data.groupby('sum_gamerounds').mean().var()[0] 
permutation_variance = [perm_test(cookie_cats_data) for _ in range(3000)]

# %%
print('Pr(Prob) is: ', np.mean([var > observed_variance_data for var in permutation_variance]))

# %%
#  Using stats.f.oneway
f_value, p_value = stats.f_oneway(version_data['gate_30'], version_data['gate_40'])
print("P value using stats.f.oneway: ",p_value)
print("F value using stats.f.oneway: ",f_value)

# %%
#  Derivation using F-stastic
mu = version_data.mean()
group_mu = mu.mean()
print("Group wise mean of data: ", mu.values)
print("Grand mean of data: ", group_mu)

# %%
degree_freedom = 89398
group_degree_freedom = 1 

# %%
# Sum of Squares (SS) of error, group wise
temp = version_data - mu
temp = temp**2
temp = temp.sum().sum()
SS = 1 * temp
temp

# %%
# # Mean of Sum of squares of error group wise
MSS=SS / degree_freedom 
MSS

# %%
# # Sum of squares of error for whole data
GSS = ((mu - group_mu) ** 2).sum() * 2
temp = (mu - group_mu) ** 2
GSS

# %%
# Mean of Sum of squares of error group wise
GMSS = GSS / group_degree_freedom 
GMSS

# %%
# F value
F_value = GMSS / MSS
F_value

# %%
def perm_test(df):
    df = df.copy()
    df['sum_gamerounds'] = np.random.permutation(df['sum_gamerounds'].values)
    tmp=df.set_index("version")
    df2=tmp.loc["gate_30"].reset_index()
    df2["gate_40"]=tmp.loc["gate_40"].reset_index()["sum_gamerounds"]
    df2["gate_30"]=tmp.loc["gate_30"].reset_index()["sum_gamerounds"]
    df2=version_data.iloc[:,5:]
    mu=df2.mean()
    SS=((df2-mu)**2).sum().sum()
    MSS=SS/degree_freedom
    GSS=((mu - group_mu)**2).sum()*2
    GMSS=GSS / group_degree_freedom 
    F_value_perm = GMSS/MSS
    return F_value_perm
permutation_variance = [perm_test(cookie_cats_data) for _ in range(3000)]

# %%
print('Pr(Prob) is: ', np.mean([fv > F_value for fv in permutation_variance]))

# %%
# Derivation using ANOVA table
model = ols("sum_gamerounds ~ version", data=cookie_cats_data).fit()
anova_table = sm.stats.anova_lm(model, typ=2)
anova_table

# %% [markdown]
# #OBSERVATION AND RESULTS
# 
# 
# Since the p-Value of each of the derived techniques is higher than the alpha (i.e. 0.05) (some techniques cannot be derived), we should not reject the null hypothesis. 


