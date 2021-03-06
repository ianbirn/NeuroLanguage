import numpy as np
import os
import glob
import fnmatch
import time
#will read a training file and yield the buffer, looping thru langLabels
def readLabels(langLabels): 
	i = 0
	while i < langLabels.size:
		address = '../training_texts/' + langLabels[i] + '.txt'
		buff = []
		for row in open(address, 'r'):
			buff.append(row)
		yield np.array(buff, dtype='str'), i
		i+=1

#given numpy arr, scan each letter one by one and yield it
def getKeys(arr): 
	i = 0
	for line in arr:
		for char in line:
			yield char
			
			
#this gen function will scan through the testing_files and return buffer and file name
def readTestFiles(path): 
	pattern = '*.txt'
	for textFile in os.listdir(path):
		if fnmatch.fnmatch(textFile, pattern):
			buff=[]
			for row in open(os.path.join(path, textFile)):
				buff.append(row)
			yield np.array(buff, dtype='str'), textFile
	
def readTrainingFiles(path): 
	pattern = '*.csv'
	for textFile in os.listdir(path):
		if fnmatch.fnmatch(textFile, pattern):
			with open(os.path.join(path, textFile)) as thefile:
				buff = np.loadtxt(thefile, delimiter=',')
				yield buff, textFile



#this function will generate a completely random array of 1's and -1's of
# size D
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

#given a dictionary, a Key, and D, if key exists in dictionary return value, if not
# create a new hypervector and enter it into the dictionary as a new key and return the new
# dictionary and new hypervector
def lookupItemMemory(itemM, key, D):
	if key in itemM:
		randomHV = itemM[key]
	else:
		itemM[key] = genRandomHV(D)
		randomHV = itemM[key]
	
	return itemM, randomHV

#core function. takes hypervectors for letters N at a time and circularly shifts them down
#and to the right. the N block is multiplied and added to the sumHV to create the languageHV
def computeSumHV(text, itemMemory, N, D):
	block = np.zeros((N, D), dtype='int')
	sumHV = np.zeros((1, D), dtype='int')
	i = 0
	for key in getKeys(text):
		block = np.roll(block, 1, [0,1])
		
		itemMemory, block[0,:] = lookupItemMemory(itemMemory, key, D)
		
		if i >= N:
			nGrams = block[0, :]
			for j in range(1, N, 1):
				nGrams = np.multiply(nGrams, block[j, :])
			sumHV = np.add(sumHV, nGrams)
		
		i+=1
	return itemMemory, sumHV

#helper function to turn a language hypervector into a binarized version
def binarizeHV(v):
	threshold = 0
	vsize = v.size
	for i in range(0,vsize):
		if v[0,i] > threshold:
			v[0,i] = 1
		else:
			v[0,i] = -1
	return v

#compute the similarity between two hypervectors
def cosAngle(u, v):
	cos = np.dot(u, v) / (np.linalg.norm(u) * np.linalg.norm(v))
	return cos


def buildLanguageHV(iM, langHV, langLabels, N, D):
	langAM={}
	for data, i in readLabels(langLabels):
		iM, langHV = computeSumHV(data, iM, N, D)
		langAM[langLabels[i]] = langHV
		print('file read')
		
	print(iM)
	print(langAM)
	return iM, langAM

#read testing files and calculate the similarities between them and the 
#language hypervectors and output the accuracy
def testHV(iM, langAM, N, D, langLabels, s):
	accuracy={}
	
	iMn, textHV = computeSumHV(s, iM, N, D)
	textHV = binarizeHV(textHV)
	
	if np.not_equal(iM, iMn):
		print('------NEW ITEM IN TEST SENTENCE-----')
	else:
		maxAngle = -1
		for label in langLabels:
			angle = cosAngle(langAM[label], textHV[0,:])
			accuracy[label] = angle
		
	
	## sort the list
	sortAccuracy = sorted(accuracy.items(), key=lambda kv:(kv[1], kv[0]), reverse=True)
	print(sortAccuracy)
	return sortAccuracy[0]
	
