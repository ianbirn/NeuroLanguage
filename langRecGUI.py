from tkinter import *
from tkinter import ttk
import ctypes
import subprocess
import os
from threading import Thread
from decimal import Decimal

so_file = "/home/pi/Desktop/internship/main.so"    #CONNECTION TO C CODE
main = ctypes.CDLL(so_file)

root = Tk()
root.title("Language Recognition")

#import subprocess
#subprocess.call(["gcc", "/home/pi/Desktop/internship/main.c"])
#tmp = subprocess.call("./a.out")

main.program.restype = ctypes.c_double
main.program.argtypes = [ctypes.c_int, ctypes.c_int]

main.sentence.restype = ctypes.c_int
main.sentence.argtypes = [ctypes.c_int, ctypes.c_int]

main.sentencePercentage.restype = ctypes.c_double
main.sentencePercentage.argtype = [ctypes.c_int, ctypes.c_int]

N = 3
D = 5000
langNum = 22
choice = 1
list1 = ["Afrikaans", "Bulgarian", "Czech", "Danish", "Dutch", "German", "English", "Estonian", "Finnish", "French", "Greek", "Hungarian", "Italian", "Latvian", "Lithuanian", "Polish", "Portuguese", "Romanian", "Slovak", "Slovenian", "Spanish", "Swedish", "error"]
l = IntVar()
r = IntVar()
r.set("2")
count = 1

def button_detect():
    new_path = "/home/pi/Desktop/pyFile.txt"
    entryFile = open(new_path, "w")
    if len(e.get()) == 0:
        ans.config(text="Please enter something first")
        return
    entryFile.write(e.get())
    entryFile.close()
    langNum = main.sentence(N, D)
    percentage = main.sentencePercentage(N, D)
    percentage = round(percentage, 2)
    if langNum == -1:
        ans.config(text="Please train that setting first!")
    elif langNum == -2:
        ans.config(text="Please train a setting first!")
    else:
        lang = list1[langNum]
        if percentage < 60:
            ans.config(text="My guess: " + lang + " " + str(percentage) + "% match", fg="red")
        elif percentage < 80:
            ans.config(text="My guess: " + lang + " " + str(percentage) + "% match", fg="yellow")
        elif percentage <= 100:
            ans.config(text="My guess: " + lang + " " + str(percentage) + "% match", fg="green")
        global survey
        survey = Label(root, text="Was I correct?")
        survey.grid(row=3, column=0, padx=15, pady=15)
        global yesButton
        global noButton
        yesButton = Button(root, text="Y", command=lambda: survey_buttons(1))
        yesButton.grid(row=3, column=1)
        noButton = Button(root, text="N", command=lambda: survey_buttons(2))
        noButton.grid(row=3, column=2)
        global surveyReaction
        surveyReaction = Label(root, text="")
        surveyReaction.grid(row=4, column=1)
      
def run_threading2():
    for btn in buttons:
        btn['state'] = 'disabled'
        
    progress_train.start(interval=10)
    print('started training')
    button_detect()
    progress_train.stop()
    print('ended training')
    
    for btn in buttons:
        btn['state'] = 'normal'
        
def real_detect():
    Thread(target= run_threading2).start()
    yesButton.destroy()
    noButton.destroy()
    surveyReaction.destroy()
    survey.destroy()

def button_clear():
    e.delete(0, END)
    yesButton.destroy()
    noButton.destroy()
    surveyReaction.destroy()
    survey.destroy()
    
def button_train():
    data, values = os.pipe()
    valuestr = str(N) + " " + str(D) + "\n"
    os.write(values, bytes(valuestr, "utf-8"));
    os.close(values)
    s = subprocess.check_output("gcc -O5 main.c -o main -lm;./main", stdin = data, shell=True)
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
    
def kill_window():
    newWindow.destroy()
    yesButton.destroy()
    noButton.destroy()
    surveyReaction.destroy()
    survey.destroy()
    
def survey_buttons(choice):
    if choice == 1:
        yesButton.destroy()
        noButton.destroy()
        surveyReaction.destroy()
        survey.destroy()
    elif choice == 2:
        count = 1
        global newWindow
        newWindow = Toplevel(root)
        newWindow.title("Improvements")
        surveyTitle = Label(newWindow, text="Please select the correct language manually")
        surveyTitle.grid(row=0, column=0, padx=10, pady=10)
        surveyTitle.config(font=14)
        for name in list1:
            if name == "error":
                break
            Radiobutton(newWindow, text=name, variable=l, value=count).grid(row=count, column=0, sticky='w')
            count = count +1
        submit = Button(newWindow, text="Submit", command=kill_window).grid(row=25, column=0, padx=10, pady=10)
    else:
        surveyReaction.config(text="")
        
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
    
progress_train = ttk.Progressbar(root, orient=HORIZONTAL, length=200, mode='indeterminate')
progress_train.grid(row=1, column=4, padx=15, pady=15)
    
Radiobutton(root, text="Low Function", variable=r, value=1, command=lambda: mode(r.get())).grid(row=0, column=3)
Radiobutton(root, text="Medium Function", variable=r, value=2, command=lambda: mode(r.get())).grid(row=1, column=3)
Radiobutton(root, text="High Function", variable=r, value=3, command=lambda: mode(r.get())).grid(row=2, column=3)

writeFileB = Button(root, text="Detect Language", command = real_detect, padx=50, pady=10)
writeFileB.grid(row=1, column=0)
writeFileB.config(font=14)

clearB = Button(root, text="Clear", command = button_clear, padx=30, pady=10)
clearB.grid(row=1,column=1, columnspan=2)

trainB = Button(root, text="Train Me!", command = real_training)
trainB.grid(row=0, column=4)
trainB.config(font=14)

ans = Label(root, text="My guess: ")
ans.grid(row=2, column=0, padx=10, pady=10)

done = Label(root, text=" ")
done.grid(row=2, column=4)
e = Entry(root, borderwidth=5, width=40)
e.grid(row=0, column=0, columnspan=3, padx=10, pady=10)

#BUTTON MATRIX
buttons = [writeFileB, clearB, trainB]


root.mainloop()

