from tkinter import *
from tkinter import ttk
import ctypes
import subprocess
import os
import time
from threading import Thread
from pythonFunctions import *

THIS_FOLDER = os.path.dirname(os.path.abspath('main.so'))
my_file = os.path.join(THIS_FOLDER, 'main.so')
print(my_file)

so_file = ""    #CONNECTION TO C CODE
main = ctypes.CDLL(my_file)

root = Tk()
root.title("Language Recognition")

#import subprocess
#subprocess.call(["gcc", "/home/pi/Desktop/internship/main.c"])
#tmp = subprocess.call("./a.out")

main.program.restype = ctypes.c_double
main.program.argtypes = [ctypes.c_int, ctypes.c_int]

main.sentence.restype = ctypes.c_int
main.sentence.argtypes = [ctypes.c_int, ctypes.c_int]

langLabels = np.array(['afr', 'bul', 'ces', 'dan', 'nld', 'deu', 'eng', 'est', 'fin', 'fra', 'ell', 'hun', 'ita', 'lav', 'lit', 'pol', 'por', 'ron', 'slk', 'slv', 'spa', 'swe'])

N = 3
D = 5000
langNum = 22

iM = {}
langAM = {}

def button_run():
    msg = Label(root, text= str(main.program(N, D)) + "%")
    msg.grid(row=1, column=0)

def button_write():
    new_path = os.path.join(THIS_FOLDER, 'pyfile.txt')
    entryFile = open(new_path, "w")
    entryFile.write(e.get())
    entryFile.close()
    iM={}
    langAM={}
    if var.get() == 'Python':
        path = 'cachedTraining/' + str(D) + '_' + str(N)
        if not os.path.isdir(path):
            print('folder does not exist!')
            exit()
	
        #read in the training files
        for arr, name in readTrainingFiles(path + '/'):
            if (name[0:2] == 'im'):
                newKey = os.path.splitext(name)[0][3:]
                iM[newKey] = arr
            if (name[0:2] == 'la'):
                newKey = os.path.splitext(name)[0][3:]
                langAM[newKey] = arr
	
        #print(iM)
        #print(langAM)
        #testing the function
        
        lang = testHV(iM, langAM, N, D, langLabels, str(e.get()))
        print(lang)
        ans.config(text='My guess: ' + lang[0])
    else:	
        if langNum == -1:
            ans.config(text="Please train that setting first!")
        elif langNum == -2:
            ans.config(text="Please train a setting first!")
        else:
            list1 = ["afrikaans", "bulgarian", "czech", "danish", "dutch", "german", "english", "estonian", "finnish", "french", "greek", "hungarian", "italian", "latvian", "lithuanian", "polish", "portuguese", "romanian", "slovak", "slovenian", "spanish", "swedish", "error"]
            lang = list1[langNum]
            ans.config(text="My guess: " + lang)
    
def button_clear():
    e.delete(0, END)
    
def button_train():
    iM={}
    langAM={}
    if var.get() == 'Python':
	    iM, langAM = buildLanguageHV(iM, langAM, langLabels, N, D)
	
		#check if folder exists, create or clear it
	    path = 'cachedTraining/' + str(D) + '_' + str(N)
	    if not os.path.isdir(path):
		    os.mkdir(path)
	
		#write itemMemory
	    for key, value in iM.items():
		    filename = 'im_' + key + '.csv'
		    with open(path + '/' + filename, 'w') as thefile:
			    np.savetxt(thefile, value, delimiter=',')

	    for key, value in langAM.items():
		    filename = 'la_' + key + '.csv'
		    with open(path + '/' + filename, 'w') as thefile:
			    np.savetxt(thefile, value, delimiter=',')
    else: 
	    data, values = os.pipe()
	    valuestr = str(N) + " " + str(D) + "\n"
	    os.write(values, bytes(valuestr, "utf-8"));
	    os.close(values)
	    s = subprocess.check_output("gcc -O5 c/main.c -o main -lm;./main", stdin = data, shell=True)
	    print(s.decode("utf-8"))
    
    if N==5:
        done.config(text="Done training low setting!")
    elif N==3:
        done.config(text="Done training medium setting!")
    elif N==4:
        done.config(text="Done training high setting!")
    else:
        done.config(text="check your code you did something wrong")
    
def run_threading():
    for btn in buttons:
        btn['state'] = 'disabled'
        
    progress_train.start(interval=10)
    print('started training')
    button_train()
    progress_train.stop()
    print('ended training')
    
    for btn in buttons:
        btn['state'] = 'normal'
    
def real_training():
    Thread(target=run_threading).start()
    
def mode(value):
    global N
    global D
    if value == 1:
        N = 5
        D = 1000
    elif value == 2:
        N = 3
        D = 5000
    elif value == 3:
        N = 4
        D = 10000
    
r = IntVar()
r.set("2")

progress_train = ttk.Progressbar(root, orient=HORIZONTAL, length=200, mode='indeterminate')
progress_train.grid(row=1, column=4, padx=15, pady=15)

#runProgramB = Button(root, text="Click for Accuracy", command = button_run)
#runProgramB.grid(row=0, column=0)
    
Radiobutton(root, text="Low Function", variable=r, value=1, command=lambda: mode(r.get())).grid(row=0, column=3)
Radiobutton(root, text="Medium Function", variable=r, value=2, command=lambda: mode(r.get())).grid(row=1, column=3)
Radiobutton(root, text="High Function", variable=r, value=3, command=lambda: mode(r.get())).grid(row=2, column=3)

#dropdown menu
options = ['C','Python']
var = StringVar(root)
var.set(options[0])
optmenu = OptionMenu(root, var, *options).grid(row=3, column=4)


writeFileB = Button(root, text="Detect Language", command = button_write, padx=50, pady=10)
writeFileB.grid(row=1, column=0)
writeFileB.config(font=14)

clearB = Button(root, text="Clear", command = button_clear, padx=30, pady=10)
clearB.grid(row=1,column=1)

trainB = Button(root, text="Train Me!", command = real_training)
trainB.grid(row=0, column=4)
trainB.config(font=14)

ans = Label(root, text="My guess: ")
ans.grid(row=2, column=0, padx=10, pady=10)

done = Label(root, text=" ")
done.grid(row=2, column=4)

e = Entry(root, borderwidth=5, width=40)
e.grid(row=0, column=0, columnspan=2, padx=10, pady=10)

buttons = [writeFileB, clearB, trainB]


root.mainloop()

