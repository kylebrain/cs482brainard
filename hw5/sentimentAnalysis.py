# If getting errors, might need to install these packages:
# pip install nltk
# pip install gensim

# importing all necessary modules 
from nltk.tokenize import sent_tokenize, word_tokenize 
import warnings 

warnings.filterwarnings(action = 'ignore') 

import gensim 
from gensim.models import Word2Vec 


#----------------------------------------------------------------
# Tokenize the sentence into words
# 	Input: 	sent - a string 
# 	Output: data - a list containing the tokenized elements of the string
#----------------------------------------------------------------
def Tokenize(sent): 
    data = []
    for i in sent_tokenize(sent): 
        temp = [] 
        # tokenize the sentence into words 
        for j in word_tokenize(i): 
            temp.append(j.lower()) # convert to lower case
        data.append(temp) 
    return data

#----------------------------------------------------------------
# Remove the stop words from the sentence
# 	Input: 	sent  - a list of words 
#		    stops - a list of stop words
# 	Output: data  - the list of words with the stop words removed
#----------------------------------------------------------------
def removeStopWords(sent, stops):
    data = []
    for word in sent:
        if word not in stops:
            data.append(word)
    return data
            
#----------------------------------------------------------------
# Remove the puncuation from the sentence
# 	Input: 	sent  - a list of words 
#		    punc  - a list of puncuation symbols
# 	Output: data  - the list of words with the puncuation removed
#----------------------------------------------------------------
def removePunctuation(sent, punc):
    data = []
    for word in sent:
        if word not in punc:
            data.append(word)
    return data  

#----------------------------------------------------------------
# Get the sentiment of the sentence
# 	Input: 	model   - the word2vec model trained on the input data
#           sent    - a list of words 
#		    good_ws - a list of words that have positive sentiment
#           bad_ws  - a list of words that have negative sentiment
# 	Output: sentiment  - the sentiment of the sentence (a number)
#----------------------------------------------------------------
def GetSentiment(model, sent, good_ws, bad_ws):
    # STUB
    return sentiment


#================================================================

if __name__ == "__main__":

    # Reads ‘corupus.txt’ file 
    sample = open("corpusLong.txt", "r") 
    s = sample.read() 

    # Replaces escape character with space 
    f = s.replace("\n", " ") 

    # iterate through each sentence in the file 
    data = Tokenize(f)
    
    # Create word2vec model 
    model = gensim.models.Word2Vec(data, min_count = 1, size = 100, window = 5, sg = 1)

    # define test sentences
    s1 = "It is patronising, illogical and nonsensical."
    s2 = "A complete and total disaster of a movie."
    s3 = "It is remarkable for admirable direction and brilliant acting."
    s4 = "The story is delightful, entertaining, and moving."

    # First tokenize the sentences:
    s1T = Tokenize(s1)
    s2T = Tokenize(s2)
    s3T = Tokenize(s3)
    s4T = Tokenize(s4)

    # Need to flatten the sentences
    s1_cleaned = [item for sublist in s1T for item in sublist]
    s2_cleaned = [item for sublist in s2T for item in sublist]
    s3_cleaned = [item for sublist in s3T for item in sublist]
    s4_cleaned = [item for sublist in s4T for item in sublist]

    # remove stop words and puncuation
    stops = ["the", "and", "in", "i", "a", "or", "as", "of", "for", "to", "my", "is", "are", "by", "on", "also", "this", "it", "at", "they", "movie"]
    punc = [".", "(", ")", ",", "!"]
    s1_cleaned = removeStopWords(s1_cleaned, stops); s1_cleaned = removePunctuation(s1_cleaned, punc)
    s2_cleaned = removeStopWords(s2_cleaned, stops); s2_cleaned = removePunctuation(s2_cleaned, punc)
    s3_cleaned = removeStopWords(s3_cleaned, stops); s3_cleaned = removePunctuation(s3_cleaned, punc)
    s4_cleaned = removeStopWords(s4_cleaned, stops); s4_cleaned = removePunctuation(s4_cleaned, punc)
    # print(s1_cleaned); print(s2_cleaned); print(s3_cleaned); print(s4_cleaned);

    # define the set of good and bad words to use for comparison when getting sentiment
    good_words = ['excellent','hilarious','amazing', 'engaging', 'love']
    bad_words = ['long','terrible','absurd','arrogant', 'hate']

    # get the sentiment of the 4 sentences - print out each and explain if you agree with the sentiment
    # STUB

