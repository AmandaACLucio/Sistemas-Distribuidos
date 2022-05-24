import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os

def choose_tuple(np, nc):

    result = {(1, 1): 0, (1, 2): 1, (1, 4): 2, (1, 8): 3, (1, 16): 4, (2, 1): 5, (4, 1): 6, (8, 1): 7, (16, 1): 8}

    return result[(np, nc)]


def dataframe_csv(data_file):

    df = pd.read_csv(data_file)

    df = df.groupby([ 'Np', 'Nc', 'N']).mean().reset_index()

    df['graph_index'] = df.apply(lambda x: choose_tuple(
        np=x['Np'], nc=x['Nc']), axis=1)

    df['graph_index'] = df.apply(lambda x: "("+str(x['Np'])+","+str(x['Nc'])+")", axis=1)

    return df

def save_graph(image, name):

    image.get_figure().savefig(f'./Projeto_2/2. Semaforo/ {name}.png')


def build_graph(df):

    fig = df.pivot(index='graph_index', columns='N',
                   values='time').plot(title="Número de Threads x Tempo")
    fig.set_xlabel("Np, Ns")
    fig.set_ylabel("Tempo (s)")
    return fig

if __name__ == "__main__":

    data_file = './Results.csv'
    data_file = os.path.join(os.path.dirname(__file__), data_file)


    df = dataframe_csv(data_file)

    image = build_graph(df)

    save_graph(image, "result")