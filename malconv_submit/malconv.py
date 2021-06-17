from tensorflow.keras.models import Model
from tensorflow.keras.layers import Dense, Embedding, Conv1D, multiply, GlobalMaxPool1D, Input, Activation
from tensorflow.keras import regularizers


def get_malconv():
    
    max_len = 200000
    win_size = 500
    x = Input((max_len,))
    emb = Embedding(256, 8)(x)
    conv1 = Conv1D(kernel_size=(win_size), filters=128, strides=(win_size), padding='same')(emb)
    conv2 = Conv1D(kernel_size=(win_size), filters=128, strides=(win_size), padding='same')(emb)
    a = Activation('sigmoid', name='sigmoid')(conv2)
    mul = multiply([conv1, a])
    a = Activation('relu', name='relu')(mul)
    p = GlobalMaxPool1D()(a)
    d = Dense(64, activation='relu', kernel_regularizer=regularizers.l2(0.01),
                activity_regularizer=regularizers.l1(0.01))(p)
    out = Dense(4, activation=None)(d)
    return Model(x, out)


def get_twohead():
    
    max_len = 200000
    win_size = 500
    x = Input((max_len,))
    emb = Embedding(256, 8)(x)
    conv1 = Conv1D(kernel_size=(win_size), filters=128, strides=(win_size), padding='same')(emb)
    conv2 = Conv1D(kernel_size=(win_size), filters=128, strides=(win_size), padding='same')(emb)
    a = Activation('sigmoid', name='sigmoid')(conv2)
    mul = multiply([conv1, a])
    a = Activation('relu', name='relu')(mul)
    p = GlobalMaxPool1D()(a)
    d = Dense(64, activation='relu', kernel_regularizer=regularizers.l2(0.01),
                     activity_regularizer=regularizers.l1(0.01))(p)
    out1 = Dense(4, activation=None)(d)
    out2 = Dense(4, activation=None)(d)
    return Model(inputs=x, outputs=[out1, out2])