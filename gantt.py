#%%
import plotly.figure_factory as ff
import pandas as pd
import os

pathdir = './data/gantt'
files = os.listdir(pathdir)

RED = 'rgb(255, 0, 0)'
GREEN = 'rgb(0, 255, 0)'
BLUE = 'rgb(0, 0, 255)'
BLACK = 'rgb(100, 100, 100)'
DARKGREEN = 'rgb(50, 100, 50)'
DARKBLUE = 'rgb(100, 50, 50)'
DARKRED = 'rgb(50, 50, 100)'

colors = {'IDLE':'rgb(100, 100, 100)'}

for filename in files:
    if filename.rfind('txt') < 0:
        continue
    f = open(pathdir + '/' + filename,'r')
    data = f.read()
    f.close()

    #data = data.replace(' ',',')
    data = data.split()

    print(data)
    gantt_dict = []
    i = 0
    start = 0
    finish = 0
    timer = 0
    pid = ''

    for i in range(0,len(data)-1):
        # pid = data[i]
        if data[i] == '0':
            pid = 'IDLE'
        else:
            pid = 'P' + data[i]
            colors[pid] = DARKGREEN

        if data[i] != data[i+1]:
            finish = timer+1
            gantt_dict.append(dict(Task=pid, Start=start, Finish=finish, Resource=pid))
            start = finish

        if i == len(data)-2:
            finish = timer+2
            gantt_dict.append(dict(Task=pid, Start=start, Finish=finish, Resource=pid))

        timer+=1
        i+=1
    df = pd.DataFrame(gantt_dict)
    print(df)

    fig = ff.create_gantt(df, colors=colors, index_col='Resource', show_colorbar=True,group_tasks=True)
    # fig.update_yaxes(autorange="reversed")
    fig.update_layout(title_text=filename, xaxis_type='linear')
    fig.show()
    # print(gantt_dict)