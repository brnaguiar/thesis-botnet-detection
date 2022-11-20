import numpy
import pandas

def featureNormalization(X):
    mean = numpy.mean(X, axis=0)
    std = numpy.std(X, axis=0)
    X_norm = (X-mean)/std
    return X_norm, mean, std

def computeCost(X, y, theta):
    m = len(y)
    h = numpy.dot(X, theta)
    res = h-y
    sub = numpy.sum((res)**2)
    J = (1/(2*m)) * sub
    return J

def gradientDEscent(X, y, theta, alpha, num_iters):
    m = len(y)
    J_history = []

    for i in range(num_iters):
        h = numpy.dot(X, theta)
        #////print("H_SHAPE", h.shape)
        grad = numpy.dot(X.transpose(), (h-y))
#        print((grad))#X.transpose()#h-y
        #print("GRAD_SHAPE", grad.shape)
        descent = (alpha/m)*grad
        print(descent)
         #(double) 
         #print("DESCENT_SHAPE", descent.shape)
        theta = theta - descent
        J_history.append(computeCost(X, y, theta))

    return theta, J_history

def predict(X, theta):
    h = numpy.dot(X, theta)
    return h[0]

data = pandas.read_csv("Multi_linear.txt", header=None).values 

X=data[:, 0:2]
y = data[:, 2].reshape(len(data[:, 2]), 1)
#X,mean ,std = featureNormalization(X)
#print(X)
X = numpy.append(numpy.ones((len(X), 1)), X, axis=1)
X = numpy.append(X, y, axis=1)
#print(X)
#print(y)
#pandas.DataFrame(X.round(5)).to_csv("normalized_dataset.csv", index=False)
pandas.DataFrame(X).to_csv("int_dataset.csv", index=False)
#theta = numpy.zeros((X.shape[1], 1))
#print("CUSTO: ", computeCost(X, y, theta))

#theta, J_history = gradientDEscent(X, y, theta, 0.01, 800) #0p2
    #print("h(x) ="+str(round(theta[0,0],2))+" + "+str(round(theta[1,0],2))+"x1 + "+str(round(theta[2,0],2))+"x2")


#x_sample = [1650,3]
#feature normalisation of x_sample
#x_sample=(x_sample-mean)/std
#x_sample=numpy.append(numpy.ones(1),x_sample)
#predict3 = predict(x_sample, theta)
#print(predict3)
#print("OLA", predict3)
#print("For size of house = 1650, Number of bedroom = 3, we predict a house value of $"+str(numpy.round(predict3,0)))
