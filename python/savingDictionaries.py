import numpy as np
import csv
import os
import fnmatch

def readTrainingFiles(path): 
	pattern = '*.csv'
	for textFile in os.listdir(path):
		if fnmatch.fnmatch(textFile, pattern):
			buff = np.loadtxt(os.path.join(path, textFile), dtype='int', delimiter=',')
			yield buff, textFile



def genRandomHV (D):
	if D % 2:
		print('dimension is odd!')
	else:
		randomIndex = np.random.permutation(D)
		randomHV = np.zeros((1, D), dtype='int')
		half = D/2
		randomHV[0, np.where(randomIndex < half)] = 1
		randomHV[0, np.where(randomIndex >= half)] = -1
	return randomHV

if __name__ == '__main__':
	
	mydict = {'a':genRandomHV(10000),
				'b':genRandomHV(10000)}
	
	lang = {'afr': genRandomHV(10000),
				'bul':genRandomHV(10000)}
	
	print(mydict)	
	print(lang)
	for key, value in mydict.items():
		filename = 'im_' + key + '.csv'
		with open('cachedTraining/' + filename, 'w') as thefile:
			np.savetxt(thefile, value, delimiter=',')
				

	for key, value in lang.items():
		filename = 'la_' + key + '.csv'
		with open('cachedTraining/' + filename, 'w') as thefile:
			np.savetxt(thefile, value, delimiter=',')



	newDict = {}
	newLang ={}
	for arr, name in readTrainingFiles('cachedTraining/'):
		if (name[0:2] == 'im'):
			newKey = os.path.splitext(name)[0][3:]
			newDict[newKey] = arr
		if (name[0:2] == 'la'):
			newKey = os.path.splitext(name)[0][3:]
			newLang[newKey] = arr
	print(newDict)
	print(newLang)
