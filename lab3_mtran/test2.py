sample = [0,1,2,3,-1,2,4]
def number_of_occurrences(element, sample): 
    n = 0 
    i = 0
    while i < len(sample):
         if i == element:
            n = n + 1
        i = i + 1
    return n
print("First way:")
print(number_of_occurrences(2,sample))

# Second way
print("Second way")
print(sample.count(2))