import tkinter as tk
import tkinter.ttk as ttk
import json
from sys import platform

app = tk.Tk()
app.geometry("480x360")
app.attributes('-fullscreen',True)
app.configure(background='black')

def write():
    for child in app.winfo_children():
        child.destroy()

    try:
        if platform == "linux":
            f = open('/home/pi/FechaduraIfrs/TelaFechaduraIf/input.json','r')
        else:
            f = open('input.json','r')
        input = json.load(f)
        
        label = tk.Label(
        cursor='none',
        text=input['msg'],
        foreground=input['color'],  # Set the text color to white
        background="black",  # Set the background color to black
        font=('normal',input['size'])
        ).place(relx=.5,rely=.5,anchor='center')
        f.close()
        app.after(500, write)
    except Exception as e:
        print(e)
        app.after(2000, write)

    

write()
app.bind('<Double-1>', quit) 
app.bind('<Escape>', quit) 
app.mainloop()