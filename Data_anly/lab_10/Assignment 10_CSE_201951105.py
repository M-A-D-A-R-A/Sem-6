#CS312 - ASSIGNMENT 10
#Nishant Andoriya
#201951105

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import statsmodels.api as sm
import pylab as py
import numpy as np

location_loan_train_data = "loan-train - loan-train.csv"


loan_train_data = pd.read_csv(location_loan_train_data).fillna(0)
loan_train_data_applicant_income = loan_train_data["ApplicantIncome"].to_numpy()

# Mean and standard deviation of the loan data
mean_applicant_income = np.mean(loan_train_data_applicant_income)
standard_deviation_applicant_income = np.std(loan_train_data_applicant_income)
print("Mean of applicants income: ", mean_applicant_income)
print("Standard deviation of applicant income: ", standard_deviation_applicant_income)

# Plot the histogram of the Applicant’s Income.
loan_train_data_applicant_income = np.diff(np.log(loan_train_data_applicant_income[loan_train_data_applicant_income > 0]))
loan_train_data_applicant_income=(loan_train_data_applicant_income - np.mean(loan_train_data_applicant_income))/np.std(loan_train_data_applicant_income)
sns.histplot(loan_train_data_applicant_income,kde=True)
plt.xlabel("z-score")

# Check the distribution of Applicant’s Income is a Normal Distribution or not using QQ plot.
sm.qqplot(loan_train_data_applicant_income, line ='45')
py.show()