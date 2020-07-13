from tkinter import *
from tkinter import ttk
import ctypes
import subprocess
import os
from threading import Thread
from decimal import Decimal

def next_button():
    awesome.destroy()
    continueB.destroy()
    train = Label(text="\n**Before using the program for the first time,\n remember to train a function mode**\n")
    train.pack()
    color = Label(text="The program offers a color coding method based\n on how confident it is:", font='bold')
    color.pack()
    red = Label(text="\nRed indicates the lowest confidence\n", fg='red')
    red.pack()
    orange = Label(text="Orange shows that the program is pretty confident\n", fg='dark orange')
    orange.pack()
    green = Label(text="Green means it is almost fully certain of its guess\n", fg="green")
    green.pack()
    now = Label(text="If you're ready, please exit out \nand restart the program to begin!")
    now.pack()
    

def sharedlibfunc():
    subprocess.call("cc -fPIC -shared -o main.so main.c", shell=True)
    createB.destroy()
    welcome.destroy()
    since.destroy()
    click.destroy()
    open('first_time.txt', 'a').close()
    global awesome
    awesome = Label(root, text="\n\nAwesome! Now let's learn how to use the software.\n")
    awesome.pack()
    global continueB
    continueB = Button(root, text="Continue->", command=next_button)
    continueB.pack()
    

if os.path.isfile("./first_time.txt") == False:
    root = Tk()
    root.title("Set up")
    root.geometry('415x400')
    welcome = Label(root, text="Welcome", font=("Courier", 18))
    welcome.pack()
    since = Label(root, text="\nSince this is your first time,\nlet's get set up.")
    since.pack()
    click = Label(root, text="First let's create the shared library,\nclick the button below!\n\n")
    click.pack()
    createB = Button(root, text="Create", command=sharedlibfunc)
    createB.pack()
    root.mainloop()
    
else:
    prg = Tk()
    prg.title("Language Recognition")

    dirstring = os.path.abspath(os.getcwd())
    so_file = dirstring + "/main.so"          #CONNECTION TO C CODE
    main = ctypes.CDLL(so_file)

    #import subprocess
    #subprocess.call(["gcc", "/home/pi/Desktop/internship/main.c"])
    #tmp = subprocess.call("./a.out")

    main.program.restype = ctypes.c_double
    main.program.argtypes = [ctypes.c_int, ctypes.c_int]

    main.sentence.restype = ctypes.c_int
    main.sentence.argtypes = [ctypes.c_int, ctypes.c_int]

    main.sentencePercentage.restype = ctypes.c_double
    main.sentencePercentage.argtypes = [ctypes.c_int, ctypes.c_int]

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
        new_path = "pyFile.txt"
        if os.path.isfile(new_path) == False:
            print("Error pyFile.txt is missing\n")
        entryFile = open(new_path, "w")
        if len(e.get()) == 0:
            ans.config(text="Please enter something first", fg="black")
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
                ans.config(text="My guess: " + lang + " " + str(percentage) + "% match", fg="red", font='bold')
            elif percentage < 80:
                ans.config(text="My guess: " + lang + " " + str(percentage) + "% match", fg="dark orange", font='bold')
            elif percentage <= 100:
                ans.config(text="My guess: " + lang + " " + str(percentage) + "% match", fg="green", font='bold')
            #global survey
            #survey = Label(prg, text="Was I correct?")
            #survey.grid(row=3, column=0, padx=15, pady=15)
            #global yesButton
            #global noButton
            #yesButton = Button(prg, text="Y", command=lambda: survey_buttons(1))
            #yesButton.grid(row=3, column=1)
            #noButton = Button(prg, text="N", command=lambda: survey_buttons(2))
            #noButton.grid(row=3, column=2)
            #global surveyReaction
            #surveyReaction = Label(prg, text="")
            #surveyReaction.grid(row=4, column=1)
            
          
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

    def button_clear():
        e.delete(0, END)
        
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
            newWindow = Toplevel(prg)
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
        
    progress_train = ttk.Progressbar(prg, orient=HORIZONTAL, length=200, mode='indeterminate')
    progress_train.grid(row=1, column=4, padx=15, pady=15)
        
    Radiobutton(prg, text="Low Function", variable=r, value=1, command=lambda: mode(r.get())).grid(row=0, column=3)
    Radiobutton(prg, text="Medium Function", variable=r, value=2, command=lambda: mode(r.get())).grid(row=1, column=3)
    Radiobutton(prg, text="High Function", variable=r, value=3, command=lambda: mode(r.get())).grid(row=2, column=3)

    writeFileB = Button(prg, text="Detect Language", command = real_detect, padx=50, pady=10)
    writeFileB.grid(row=1, column=0)
    writeFileB.config(font=14)

    clearB = Button(prg, text="Clear", command = button_clear, padx=30, pady=10)
    clearB.grid(row=1,column=1, columnspan=2)

    trainB = Button(prg, text="Train Me!", command = real_training)
    trainB.grid(row=0, column=4)
    trainB.config(font=14)

    ans = Label(prg, text="My guess: ", font='bold')
    ans.grid(row=2, column=0, padx=10, pady=10)

    done = Label(prg, text=" ")
    done.grid(row=2, column=4)
    e = Entry(prg, borderwidth=5, width=40)
    e.grid(row=0, column=0, columnspan=3, padx=10, pady=10)

    #BUTTON MATRIX
    buttons = [writeFileB, clearB, trainB]

    prg.mainloop()

