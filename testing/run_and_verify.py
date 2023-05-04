import os
import datetime

def verify (file1, file2):
    FILE_1 = open(file1, 'r')
    FILE_2 = open(file2, 'r')
    
    DICT_1 = {}
    DICT_2 = {}

    id = ""

    while True:
        line = FILE_1.readline()
        if not line:
            break

        if line[0] == '#':
            id = line
            DICT_1[id] = []
        else :
            DICT_1[id].append(line)
    
    FILE_1.close()

    id = ""

    while True:
        line = FILE_2.readline()
        if not line:
            break

        if line[0] == '#':
            id = line
            DICT_2[id] = []
        else :
            DICT_2[id].append(line)
    
    FILE_2.close()

    for i in DICT_1:
        temp = DICT_1[i]
        temp.sort()
        DICT_1[i] = temp
    
    for i in DICT_2:
        temp = DICT_2[i]
        temp.sort()
        DICT_2[i] = temp

    
    if DICT_1 == DICT_2 :
        return True
    else :
        return False






if __name__ == '__main__':    
    os.system('g++ ../src/main.cpp -o ../src/main')
    os.system('g++ ../src/simp_algo.cpp -o ../src/simp_algo')
    os.system('g++ ../src/acTrie.cpp -o ../src/acTrie')

    path1 = "../datasets"
    path2 = "outputs"

    datasets = os.listdir(path1)
    for dataset in datasets:
        PAT = path2 + '/' + 'output_' + dataset
        if not os.path.exists(PAT):
            PA = 'mkdir ' + path2 + '/' + 'output_' + dataset  
            print(PA)          
            os.system(PA)
            print(PA)

        path3 = path1 + '/' + dataset 
        files = os.listdir(path3)
        sum_main = 0
        sum_simp = 0
        sum_acSimp = 0
        sum4 = 0
        size_1 = 0
        size_2 = 0
        for file in files:
            if file.endswith('.graph'):
                executable = '../src/main'
                input_file = '../datasets/' + dataset + '/' + file 
                output_file_1 = 'outputs/output_' + dataset + '/' + file.split('.')[0] + '_main.txt'
                command = './' + executable + ' ' + input_file + ' ' + output_file_1 + ' ' + '1'
                os.system(command)

                executable = '../src/simp_algo'
                output_file_2 = 'outputs/output_' + dataset + '/' + file.split('.')[0] + '_simpalgo.txt'
                command = './' + executable + ' < ' + input_file + ' > ' + output_file_2
                now = datetime.datetime.now().timestamp()               
                os.system(command)
                after_execution = datetime.datetime.now().timestamp()
                sum_simp += after_execution-now
                file_stats2 = os.stat(output_file_2)
                size_2 += file_stats2.st_size

                executable = '../src/acTrie'
                output_file_main = 'outputs/output_' + dataset + '/' + file.split('.')[0] + 'acTrie_main.txt'
                output_file_simp_algo = 'outputs/output_' + dataset + '/' + file.split('.')[0] + 'acTrie_simp_algo.txt'
                command_main = './' + executable + ' f ' + '< ' + output_file_1 + ' > ' + output_file_main
                command_simp_algo = './' + executable + ' f ' + '< ' + output_file_2 + ' > ' + output_file_simp_algo           
                os.system(command_main)
                now = datetime.datetime.now().timestamp()               
                os.system(command_simp_algo)
                after_execution = datetime.datetime.now().timestamp()
                sum_acSimp += after_execution-now
                

                check = verify(output_file_main, output_file_simp_algo)
                if check == False:
                    temp = output_file_main +  ' and ' + output_file_simp_algo + ' differs in result.'
                    print(temp)
                else :
                   print("Hurray! Verified")
        
                executable = '../src/main'
                input_file = '../datasets/' + dataset + '/' + file 
                output_file_3 = 'outputs/output_' + dataset + '/' + file.split('.')[0] + '_main.txt'
                command = './' + executable + ' ' + input_file + ' ' + output_file_3 + ' ' + '0'
                now = datetime.datetime.now().timestamp()    
                os.system(command)
                after_execution = datetime.datetime.now().timestamp()
                sum_main += after_execution-now
                file_stats1 = os.stat(output_file_3)
                size_1 += file_stats1.st_size


        with open("timings.txt",'a') as f:
           f.write("For dataset {} {} {} {}".format(dataset,sum_main,sum_simp,sum_simp+sum_acSimp))
           f.write("\n")
        with open("output_sizes.txt",'a') as f:
            size_1 = size_1//(1024*1024)
            size_2 = size_2//(1024*1024)
            f.write("For dataset {} {} {}".format(dataset,size_1,size_2))      
            f.write("\n")              






