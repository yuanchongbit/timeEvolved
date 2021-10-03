import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt
from sklearn.neighbors import KernelDensity
from sklearn.metrics import classification_report
from tqdm import tqdm
from tensorflow import keras
from sklearn import metrics


def pad_data(res, max_len=200000):
    length = len(res)
    if length > max_len:
        return res[ :max_len]
    elif length < max_len:
        return res + [0]*(max_len-length)
    return res


def train_data_generator(codes, labels, mode):
    
    if mode == 'train':    
        codes = codes[: 6000]
        labels = labels[: 6000]
    elif mode == 'val':    
        codes = codes[6000: ]
        labels = labels[6000: ]
    
    labels = np.eye(4)[labels]
    for idx in range(len(codes)):
        fn = codes[idx]
        fn = bytes.decode(fn)
        fn = '../../data/' + fn[8: ]
        if not os.path.isfile(fn):
            print(fn, 'not exist')
        else:
            with open(fn, 'rb') as f:
                res = f.read()
                res = [byte for byte in res]
                res = pad_data(res)
                yield res, labels[idx]
                
                
                
def train_data_loader(codes, labels):
    
    # if mode == 'train':    
    #     codes = codes[: 6000]
    #     labels = labels[: 6000]
    # elif mode == 'val':    
    #     codes = codes[6000: ]
    #     labels = labels[6000: ]
    
    labels = np.eye(4)[labels]
    res_all = []
    for idx in tqdm(range(len(codes))):
        fn = codes[idx]
        fn = '../../data/' + fn[8: ]
        if not os.path.isfile(fn):
            print(fn, 'not exist')
        else:
            with open(fn, 'rb') as f:
                res = f.read()
                res = [byte for byte in res]
                res = pad_data(res)
                res_all.append(res)
    return np.array(res_all), labels


def test_data_loader(codes, labels):
    
    labels = np.eye(4)[labels]
    res_all = []
    for idx in tqdm(range(len(codes))):
        fn = codes[idx]
        fn = '../../data/' + fn[8: ]
        if not os.path.isfile(fn):
            print(fn, 'not exist')
        else:
            with open(fn, 'rb') as f:
                res = f.read()
                res = [byte for byte in res]
                res = pad_data(res)
                res_all.append(res)
    return np.array(res_all), labels


def codes_loader(codes):
    
    res_all = []
    for idx in range(len(codes)):
        fn = codes[idx]
        fn = '../../data/' + fn[8: ]
        if not os.path.isfile(fn):
            print(fn, 'not exist')
        else:
            with open(fn, 'rb') as f:
                res = f.read()
                res = [byte for byte in res]
                res = pad_data(res)
                res_all.append(res)
    return np.array(res_all)


def path_loader(data_csv):
    
    name2label = {'trojan':0, 'virus':1, 'worm':2, 'backdoor':3}
    codes = data_csv['name'].to_list()
    labels = data_csv['label'].map(lambda x: name2label[x])
    labels = labels.to_list()
    return codes, labels    
    
    
def get_discs(model, codes, labels):
    out_1, out_2 = model.predict(codes)
    out_1 = keras.backend.softmax(out_1).numpy()
    out_2 = keras.backend.softmax(out_2).numpy()
    entropy_1 = np.max(out_1, axis=1)
    entropy_2 = np.max(out_2, axis=1)
    discs = np.abs(entropy_1 - entropy_2).reshape((-1, 1))  
    
    return discs


def plot_entropy(discs, labels, name):
    
    id_discs = discs[labels == 0].reshape(-1,1)
    ood_discs = discs[labels == 1].reshape(-1,1)
    fig, ax = plt.subplots(tight_layout=True)
    names = ['ID Entropy', 'OOD Entropy']
    X_plot = np.linspace(discs.min(), discs.max(), 1000)[:, np.newaxis]

    for i, X in enumerate([id_discs, ood_discs]):
        kde = KernelDensity(kernel='gaussian', bandwidth=(discs.max()-discs.min())/100).fit(X)
        log_dens = kde.score_samples(X_plot)
        ax.plot(X_plot[:, 0], np.exp(log_dens) / 100., '-', label=names[i])
    ax.set_title('Entropy distribution')
    ax.set_xlim(discs.min(), discs.max())
    ax.legend(loc='upper right')
    fig.savefig('./figures/' + name, dpi=500)
    plt.close(fig)
    
    
def codes_generator(codes):
    
    for idx in range(len(codes)):
        fn = codes[idx]
        fn = bytes.decode(fn)
        fn = '../../data/' + fn[8: ]
        if not os.path.isfile(fn):
            print(fn, 'not exist')
        else:
            with open(fn, 'rb') as f:
                res = f.read()
                res = [byte for byte in res]
                res = pad_data(res)
                yield res
                

def svmdata_generator(codes, labels, mode):
    
    if mode == 'train':    
        codes = codes[: 206]
        labels = labels[: 206]
    elif mode == 'val':    
        codes = codes[206: 406]
        labels = labels[206: 406]
    
    labels = np.eye(4)[labels]
    for idx in range(len(codes)):
        fn = codes[idx]
        fn = bytes.decode(fn)
        fn = '../../data/' + fn[8: ]
        if not os.path.isfile(fn):
            print(fn, 'not exist')
        else:
            with open(fn, 'rb') as f:
                res = f.read()
                res = [byte for byte in res]
                res = pad_data(res)
                yield res, labels[idx]

def test():
    print("test")