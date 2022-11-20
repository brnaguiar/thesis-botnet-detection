import pandas as pd
from sklearn.metrics import classification_report, precision_recall_fscore_support, confusion_matrix
from plotly.subplots import make_subplots
import numpy as np
import plotly.graph_objects as go
import os

def fetch_dataset(PATH, dataset):
    files = [f for f in os.listdir(PATH) if os.path.isfile(os.path.join(PATH, f))]
     
    if dataset == "normal":
        return {
            file.split(".")[0] : pd.read_csv(os.path.join(PATH, file), delim_whitespace=True) 
                for file in files if "windows-18may-19h14" not in file
        } # ds-windows-18may-19h14 = dataset that contains anti-virus software, manufacter software, etc... not good

    elif dataset == "anomaly_pings":
        return {
            file.split(".")[0] : pd.read_csv(os.path.join(PATH, file), delim_whitespace=True) 
                for file in files if "pings" in file 
        }
    elif dataset == "anomaly_nopings":
        return {
            file.split(".")[0] : pd.read_csv(os.path.join(PATH, file), delim_whitespace=True) 
                for file in files if "pings" not in file #and "08july" not in file
        }
    else:
        return {
            file.split(".")[0] : pd.read_csv(os.path.join(PATH, file), delim_whitespace=True)
                for file in files
        }

def display_histograms(datasets_normal, datasets_anomaly, pdf_name, min_value, max_value):
    allnormal = pd.concat([datasets_normal[df].copy() for df in datasets_normal]) 
    allanomaly = pd.concat([datasets_anomaly[df].copy() for df in datasets_anomaly])
    
    MIN = min_value#0
    MAX = max_value#26 #allnormal.shape[1]+1
    
    fig = go.Figure()
    fig = make_subplots(rows=int(abs(MIN-MAX)/2), cols=2, subplot_titles=tuple((allnormal.keys()[MIN:MAX]))) 

    row=1
    col=0

    for i, v in enumerate(allnormal.keys()[MIN:MAX]): 
        fig.append_trace(go.Histogram(x=[a for a in allnormal[v] if a > 0], 
                                   histnorm='probability', 
                                   name='allnormal', marker_color='blue'), row=row, col=col+1)
        #list comprehension: remove silence periods...
        fig.append_trace(go.Histogram(x=[a for a in allanomaly[v] if a > 0], 
                                   histnorm='probability', 
                                   name='allanomaly', marker_color='red'), row=row, col=col+1) 
        if (i+1) % 2 == 0: row = row + 1;
        col = (col + 1) % 2;

    #fig.update_xaxes(title_text="Average Packet Length (Bytes)")
    fig.update_yaxes(title_text="Percentage")

    fig.update_layout(barmode='overlay', height=8000, showlegend=False)
    fig.update_traces(selector=dict(name='allanomaly'), opacity=0.55)
    fig.update_traces(selector=dict(name='allnormal'), opacity=1.0)
    import plotly.io as pio
    pio.write_image(fig, pdf_name, scale=6, width=1080, height=1200)#a88
    fig.show()#########6060606060606060

#def display_2DScatter():
#    """ original """
#    datav_2d(0, 7, all_ds)
#    datav_2d(8, 15, all_ds)
#    datav_2d(16, 23, all_ds)
#    datav_2d(24 , 31, all_ds)
#    datav_2d(32, 39, all_ds)
#    datav_2d(40, 47, all_ds)
#    datav_2d(48, 55, all_ds)
 
def datav_2d(min, max, ds):
    columns2pick = ds.columns[min:max]  # 8 12
    ds = ds[columns2pick.append(pd.Index([ds.columns[-1]]))]
    columns2pick = list(columns2pick)
    index_vals = ds['label'].astype('category').cat.codes
    #print(index_vals)
    opacities = [1 if val == 0 else 0.1 for val in index_vals]
    fig = go.Figure(data=go.Splom(
        dimensions=[dict(label=name, values=ds[name]) for name in ds.iloc[:, :-1]],
        text=ds['label'],
        marker=dict(color=index_vals,
                            showscale=False, # colors encode categorical variables
                            line_color=None, line_width=0.2, opacity=opacities)))
    
    fig.update_traces(diagonal_visible=False)
    fig.update_layout(height = 900)
    fig.show()

def complete_val_threshold(scores, y, step_resolution=1000, weights=[0.5, 0.5]):
    stepsize = (max(scores) - min(scores)) / step_resolution
    epi_range = np.arange(scores.min(), scores.max(), stepsize)
    
    best_epi_0, best_epi_1, best_epi_w = 0, 0, 0
    best_f1_0, best_f1_1, best_f1_w = 0, 0, 0
    curr_p0, curr_p1, curr_pw = 0, 0, 0
    curr_r0, curr_r1, curr_rw = 0, 0, 0
    curr_c0, curr_c1, curr_cw = 0, 0, 0
    
    for epi in epi_range:
        predictions = (scores < epi)[:, np.newaxis]
        
        try: # RuntimeWarning: invalid value encountered in long_scalars --> normaly is division 0/0     
            confusion   = confusion_matrix(y, predictions) 
            precision_0 = confusion[0][0] / np.sum(confusion[:, 0])
            precision_1 = confusion[1][1] / np.sum(confusion[:, 1])
            precision_w = (weights[0]*precision_0) + (weights[1]*precision_1)
            recall_0    = confusion[0][0] / np.sum(confusion[0, :])
            recall_1    = confusion[1][1] / np.sum(confusion[1, :])    
            recall_w    = (weights[0]*recall_0) + (weights[1]*recall_1)   
            f1_0 = (2*precision_0*recall_0) / (precision_0 + recall_0)
            f1_1 = (2*precision_1*recall_1) / (precision_1 + recall_1)
            f1_w = (weights[0]*f1_0) + (weights[1]*f1_1)
            
            #print(confusion)
            
            if f1_w > best_f1_w:
                best_f1_w = f1_w   
                best_epi_w = epi
                curr_pw  = precision_w
                curr_rw = recall_w
                curr_cw = confusion

            if  f1_0 > best_f1_0:
                best_f1_0 = f1_0
                best_epi_0 = epi
                curr_p0  = precision_0
                curr_r0 = recall_0
                curr_c0 = confusion
            
            if f1_1 > best_f1_1:
                best_f1_1 = f1_1
                best_epi_1 = epi
                curr_p1  = precision_1
                curr_r1 = recall_1
                curr_c1 = confusion

        except ZeroDivisionError:
            print('Warning dividing by zero!!') 
        
    return [best_f1_0, best_epi_0, curr_p0, curr_r0,curr_c0 ],\
           [best_f1_1, best_epi_1, curr_p1, curr_r1, curr_c1],\
           [best_f1_w, best_epi_w, curr_pw, curr_rw, curr_cw]  


def val_threshold(scores, y, step_resolution=1000, weights=[0.5, 0.5]):
    stepsize = (max(scores) - min(scores)) / step_resolution
    epi_range = np.arange(scores.min(), scores.max(), stepsize)
    
    best_f1_w, best_epi_w = 0, 0
    best_confusion = None
    
    for epi in epi_range:
        predictions = (scores < epi)[:, np.newaxis]

        # RuntimeWarning: invalid value encountered in long_scalars --> normaly is division 0/0     
        with np.errstate(all='ignore'): 
            confusion   = confusion_matrix(y, predictions) 
            precision_0 = confusion[0][0] / np.sum(confusion[:, 0])
            precision_1 = confusion[1][1] / np.sum(confusion[:, 1])
            precision_w = (weights[0]*precision_0) + (weights[1]*precision_1)
            recall_0    = confusion[0][0] / np.sum(confusion[0, :])
            recall_1    = confusion[1][1] / np.sum(confusion[1, :])    
            recall_w    = (weights[0]*recall_0) + (weights[1]*recall_1)   
            f1_0 = (2*precision_0*recall_0) / (precision_0 + recall_0)
            f1_1 = (2*precision_1*recall_1) / (precision_1 + recall_1)
            f1_w = (weights[0]*f1_0) + (weights[1]*f1_1)
            
            if f1_w > best_f1_w:
                best_f1_w = f1_w   
                best_epi_w = epi
                best_confusion = confusion

    print(best_confusion)
        
    return [best_f1_w, best_epi_w]  

# PRECISION: correctly predicted class 0 / all predicted class 0
# RECALL:    correctly predicted class 0 / all actual class 0
# FPR: False Positive Rate -__- 
#   how often an actual negative instance will be classified as positive, i.e. "false alarm"...
def test_threshold(scores, y, epi, weights=[0.5, 0.5]):
    predictions = (scores < epi)[:, np.newaxis]
    
    confusion   = confusion_matrix(y, predictions) 
    
    precision_0 = confusion[0][0] / np.sum(confusion[:, 0]) # True Negative Rate (TNR)
    precision_1 = confusion[1][1] / np.sum(confusion[:, 1])
    precision_w = (weights[0]*precision_0) + (weights[1]*precision_1)
    
    recall_0    = confusion[0][0] / np.sum(confusion[0, :]) # True Positive Rate (TPR)
    recall_1    = confusion[1][1] / np.sum(confusion[1, :])    
    recall_w    = (weights[0]*recall_0) + (weights[1]*recall_1)   

    #FPR = confusion[1][0] / np.sum(confusion[:, 0])
    FPR2 = 1-precision_0

    print()
    
    f1_0 = (2*precision_0*recall_0) / (precision_0 + recall_0)
    f1_1 = (2*precision_1*recall_1) / (precision_1 + recall_1)
    f1_w = (weights[0]*f1_0) + (weights[1]*f1_1)
    
    return f1_0, f1_1, f1_w, recall_0, precision_0, FPR2, precision_w, recall_w


def feature_selection():
    from sklearn.ensemble import ExtraTreesClassifier
    from sklearn.feature_selection import SelectFromModel

    A2PATH = '../data/datasets/anomaly/adv-medium-exf/'
    A3PATH = '../data/datasets/anomaly/adv-cnc/'

    # get anomalies for current scenario
    #aux = fetch_dataset(APATH, "anomaly_pings")
    #all_anomalies_curr_scenario = pd.concat(
    #    [pd.DataFrame(np.hstack((x_anomaly_train, np.ones((x_anomaly_train.shape[0], 1)))), columns = columns)] 
    #    + [aux[d] for d in aux.keys()]
    #).sample(n=5000) #.to_numpy()
    #y_all_anomalies_curr_scenario = all_anomalies_curr_scenario['label'].sample(5000)
    #x_all_anomalies_curr_scenario = all_anomalies_curr_scenario.drop('label', axis=1)

    # get anomalies for another scenario
    aux2 = fetch_dataset(A2PATH, "anomaly_nopings")
    all_anomalies_scenario_2 = pd.concat([aux2[d] for d in aux2.keys()]).sample(n=5000) 
    y_all_anomalies_scenario_2 = all_anomalies_scenario_2['label'].sample(5000)
    x_all_anomalies_scenario_2 = all_anomalies_scenario_2.drop('label', axis=1) 

    # get anomalies for another scenario
    aux3 = fetch_dataset(A3PATH, "anomaly_nopings")
    all_anomalies_scenario_3 = pd.concat([aux3[d] for d in aux3.keys()]).sample(n=5000) 
    y_all_anomalies_scenario_3 = all_anomalies_scenario_3['label'].sample(5000)
    x_all_anomalies_scenario_3 = all_anomalies_scenario_3.drop('label', axis=1) 

    # STACK DATA 
    x_FS = pd.concat([pd.DataFrame(x_normal_train, columns = columns[:-1]).sample(5000), 
                     #x_all_anomalies_curr_scenarioaaaa,
                     x_all_anomalies_scenario_2,
                     x_all_anomalies_scenario_3], ignore_index=True)
    #print(x_FS)
    y_FS = pd.concat([pd.DataFrame(y_normal_train).sample(5000), 
                    #y_all_anomalies_curr_scenario,
                    y_all_anomalies_scenario_2,
                     y_all_anomalies_scenario_3]).to_numpy().ravel()

    x_FS, y_FS = sklearn.utils.shuffle(x_FS, y_FS) 
    clf = ExtraTreesClassifier(n_estimators=1000, max_features=2)
    clf = clf.fit(x_FS, y_FS)

    normalized_feature_importance = np.std([tree.feature_importances_ for tree in clf.estimators_], axis = 0)

    #print(zip([columns.get_loc(c) for c in columns], columns))
    normalized_feature_importance = clf.feature_importances_
    ranked_features = sorted(zip(enumerate(columns[:-1]), normalized_feature_importance), 
                             key=lambda x: x[1], reverse=True)

    best_features = []
    count = 0
    for c, f in ranked_features:
        i, c = c
        print("{} {}: {}".format(i, c, f))
        if count < 52:
            best_features.append(i)
        count= count + 1

    x_normal_train = x_normal_train[:, best_features]
    x_anomaly_train = x_anomaly_train[:, best_features]
    #x_test = x_test[:, best_features]
