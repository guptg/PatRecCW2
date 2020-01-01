# Libraries
import matplotlib.pyplot as plt
import numpy as np
import scipy.io as sio  # Used to load data stored in .mat file
from sklearn.neighbors import NearestNeighbors
from sklearn.neighbors import DistanceMetric
from sklearn.metrics import confusion_matrix
from scipy.stats import entropy
from sklearn.cluster import KMeans
import sklearn.metrics as sk_met
from scipy.optimize import linear_sum_assignment
from sklearn.cluster import AgglomerativeClustering
from scipy.special import softmax
from scipy.stats import multivariate_normal
import pdb
from sklearn.datasets import make_classification
from sklearn.mixture import GaussianMixture

n_clusters = 20

# Loading the data
mat_content = sio.loadmat('face.mat')
face_data = mat_content['X']         # Array with dims 2576x520
face_labels = mat_content['l']       # Array with dims 1x520

# Constants
num_images = face_data.shape[1]      # Number of images
D = face_data.shape[0]               # Num of pixels per image = 2576
IMAGES_PER_PERSON = 10
num_people = int(num_images / IMAGES_PER_PERSON)   # Number of people the full dataset captures


#Normalising feature vectors
norm_face_data = face_data / np.linalg.norm(face_data, axis = 0)

print('Unit vector', np.sum(norm_face_data[:, 0], axis=0))

#Splitting up into training and testing

train_data_a = face_data[:, 0:320]     #Unnormalised set
train_data_b = norm_face_data[:, 0:320] #Normalised training set
train_labels = face_labels[0, 0:320]

test_data_a = face_data[:, 320:] #Unnormalised test set
test_data_b = norm_face_data[:, 320:] #Normalised test set
test_labels = face_labels[0, 320:]
print(test_labels)

#Defining shapes needed:
SIZE_TEST = test_data_a.shape[1]
SIZE_TRAIN = train_data_a.shape[1]

train_data = train_data_b
test_data = test_data_b

print(SIZE_TEST)
print(SIZE_TRAIN)

print(train_labels)
#
"""Hungarian mapping"""


def make_cost_m(cm):
    s = np.max(cm)
    return (- cm + s)


def map_labels(true_labels, assigned_labels):
    """
    returns index_Map: index[0] is index of true labels, index[1] is clusters assigned to each class
    """

    # assigned_labels_new = assigned_labels + 1 #Add 1 to assigned labels because true labels go from 0-31

    true_labels_new = true_labels - 1  # Map true labels from 0-31 since cluster labels go from 0-31

    # print(assigned_labels)
    # print(assigned_labels_new)

    cm = sk_met.confusion_matrix(true_labels_new, assigned_labels)

    cost = make_cost_m(cm)

    index = linear_sum_assignment(cost)

    return index


"""Agglomerative Clustering"""

agg_clustering = AgglomerativeClustering(n_clusters=n_clusters).fit(train_data.T)
# print(list(clustering.labels_))

# Counting how many images belong to each cluster

counts = []
list_labels = np.ndarray.tolist(agg_clustering.labels_)

for i in range(20):
    # Count number of images assigned to each class
    count = list_labels.count(i)
    counts.append(count)
counts_array = np.asarray(counts)

# Obtained map index values
true_labels, hungarian_mapped = map_labels(train_labels,
                                           agg_clustering.labels_)  # i.e. label 1 is represented by cluster 2

# Check accuracy:
accuracy = []  # List of accuraceis
for img in range(train_data.shape[1]):
    # Obtain TRUE label

    true_label = train_labels[img] - 1  # Because in hungarian mapping training labels were mapped from 1-32 --> 0-31

    cluster_label = np.argwhere(hungarian_mapped == agg_clustering.labels_[img])
    # print(cluster_label)

    accuracy.append(bool(true_label == cluster_label))

""" Feature representation: Cluster centers as codebook and then vectors of distances to the cluster centers as feature vectors"""

# Obtain 32 agglomerative cluster centers by averaging the clusters

centroids = []
covariances = []
for i in range(n_clusters):

    index_values = np.argwhere(agg_clustering.labels_ == i)

    num_of_mems = index_values.shape[0]
    cluster_members = np.zeros((num_of_mems, 2576))  # List of array of intensiteis of images belonging to that cluster

    for i in range(index_values.shape[0]):
        member_index = index_values[i, 0]
        cluster_member = train_data[:, member_index]  # could also be test data b

        cluster_members[i, :] = cluster_member

    cluster_centroid = np.mean(cluster_members, axis=0)  # Calculate mean of cluster

    cluster_covariance = np.cov(cluster_members.T)  # Calculate covariance of cluster
    # print(cluster_covariance.shape)
    covariances.append(cluster_covariance)
    centroids.append(cluster_centroid)

centroid_array = np.asarray(centroids)

""" starting FISHER VECTORS"""
#Generate gaussian mixture, initialise using arrays
gmm = GaussianMixture(n_components=n_clusters, covariance_type='diag', means_init= centroid_array)
#Fit Gaussian on the training data
gmm.fit(train_data.T)


likelihood =  gmm.predict_proba(test_data.T)
print(likelihood.shape)
cov_gmm = gmm.covariances_.T
print(cov_gmm.shape)
mean_gmm = gmm.means_.T
print(mean_gmm.shape)
weight_gmm = gmm.weights_
print(weight_gmm.shape)

u = np.zeros((n_clusters*2576, 200))
v = np.zeros((n_clusters*2576, 200))
#
# u = np.zeros((32, 200))
# v = np.zeros((32, 200))

for img_num in range(test_data.shape[1]):

    img = test_data[:, img_num]

    dist_to_centroid = ((img.reshape(2576, 1) - mean_gmm) / cov_gmm).T

    dist_to_centroid2 = ((((img.reshape(2576, 1) - mean_gmm) / cov_gmm) - 1) **2).T

    # collapsing = np.mean(dist_to_centroid, axis = 1)

    prob = likelihood[img_num, :].reshape(n_clusters, 1)

    mult = prob * dist_to_centroid
    mult2 = prob * dist_to_centroid2

    division_by_pi = np.sqrt(weight_gmm.reshape(n_clusters, 1))
    division_by_pi2 = np.sqrt(2 * weight_gmm.reshape(n_clusters, 1))

    u_img = np.ndarray.flatten(mult / division_by_pi)
    v_img = np.ndarray.flatten(mult2 / division_by_pi2)

    # # Reduced dimensions
    # u_img = np.linalg.norm((mult / division_by_pi), axis = 1)
    # v_img = np.linalg.norm((mult2 / division_by_pi2), axis=1)

    u[:, img_num] = u_img
    v[:, img_num] = v_img

fisher_vectors = np.concatenate([u, v], axis=0)
print(fisher_vectors.shape)
# vector32 = likelihood[img_num, :] * collapsing

# u_img = vector32 / np.sqrt(weight_gmm)
#
# u[:, img_num] = u_img



""" MAP functions """



def compute_performance_scores(data, metric_name, max_rank):
    """
    data: unormalized or normalized test data split
    metric_name: input must be a string
    max_rank: dictates recall level
    """

    # Obtain model
    n_neighbors = max_rank

    neigh = NearestNeighbors(n_neighbors=n_neighbors, metric=metric_name)  # Create object
    neigh.fit(data.T)  # Transpose to obtain training data in the right format, get model

    # else:
    # metric_final = DistanceMetric.get_metric('mahalanobis', V= cov_matrix)
    # neigh = NearestNeighbors(n_neighbors=n_neighbors, metric='mahalanobis', metric_params = {'V': cov_matrix}) #Create object
    # neigh.fit(data.T) #Transpose to obtain training data in the right format, get model

    recall = np.zeros((max_rank - 1, 200))
    precision = np.zeros((max_rank - 1, 200))
    accuracy = np.zeros((max_rank - 1, 200))

    # Loop through every test image

    for img_num in range(data.shape[1]):

        correct_label = train_labels[img_num]

        test_img = data[:, img_num]
        test_img = test_img.reshape(1, -1)
        whatever, index = neigh.kneighbors(test_img)  # Obtains list of nearest neighbour for each image

        # Loop through each labels, calculate: precision, accuracy and recall
        NNrank = index[0, 1:]  # Discarding the first image
        retrieved_labels = []
        for rtr in NNrank:
            label = train_labels[rtr]
            retrieved_labels.append(label)

        for idx in range(len(retrieved_labels)):

            rank = retrieved_labels[0:(idx + 1)]  # Rank iterates through
            rank_num = len(rank)
            RelevantRetrievedTopK = rank.count(correct_label)

            if RelevantRetrievedTopK > 0:
                accuracy_value = 1
            else:
                accuracy_value = 0

            accuracy[rank_num - 1, img_num] = accuracy_value  # Not sure if this is how you do accuracy

            precision_value = RelevantRetrievedTopK / rank_num
            precision[rank_num - 1, img_num] = precision_value

            recall_value = RelevantRetrievedTopK / 9
            recall[rank_num - 1, img_num] = recall_value

    return (accuracy, precision, recall)


# WORKS FOR SIZE OF TRAINING DATA NOT TEST DATA: 320
def new_compute_MAP(precision, recall):
    """
    param: precision --> precision values for every image up till rank 200 (200x200)
    param: recall --> recall values for every image up till rank 200 (200x200)

    """

    recall_levels = [0, 1 / 9, 2 / 9, 3 / 9, 4 / 9, 5 / 9, 6 / 9, 7 / 9, 8 / 9, 1]

    NO_IMAGES = precision.shape[1]

    p_interpol = np.zeros((10, 200))

    # Looping through the images
    for img_no in range(NO_IMAGES):

        recall_img = recall[:, img_no]
        precision_img = precision[:, img_no]

        for ix, r in enumerate(recall_levels):

            count_r = np.ndarray.tolist(recall_img).count(r)

            if count_r > 0:
                # Extract index of all recall values in recall matrix with this recall
                index_values = np.argwhere(recall_img == r)

                relevant_p_vals = precision_img[index_values]

                max_p = np.max(relevant_p_vals)

                p_interpol[ix, img_no] = max_p

    map_all_imgs = np.mean(p_interpol, axis=0)
    MaP = np.mean(map_all_imgs)

    return map_all_imgs, MaP
""" MEASURE RETRIEVAL USING NEW FEATURE VECTORS """

def baseline_test(data):

    #Distance metrics
    # metric_list = ['euclidean', 'cosine', 'manhattan', 'minkowski', 'chebyshev']
    metric_list = ['euclidean']

    for metric in metric_list:

        print(data.shape[1])
        print('This is metric:', metric)
        accuracy_B, precision_B, recall_B = compute_performance_scores(data, metric, data.shape[1])
        rank_1_accuracy_B = np.mean(accuracy_B[0, :])
        rank_10_accuracy_B = np.mean(accuracy_B[9,:])
        mAP_check, MaP = new_compute_MAP(precision_B, recall_B)
        print('This is mAP:', MaP)
        print('rank 1:', rank_1_accuracy_B)
        print(' rank 10:', rank_10_accuracy_B)


print('Baseline test: ')
baseline_test(fisher_vectors)






