import random


def generator(filename,table,row,countrow,dimenze,min,max,zaokrouhleni):
        sql='INSERT INTO '+table+'('+row+') VALUES '
        fullsql=sql
        with open(filename,"w+") as f:
            for i in range(1,countrow+1):
                fullsql=fullsql+"('("
                for j in range(dimenze):
                    fullsql=fullsql+str(round(random.uniform(min,max),zaokrouhleni))
                    if(j!= (dimenze-1)):
                        fullsql=fullsql+','
                    else:
                        if(i!=countrow):
                             fullsql=fullsql+")'),\n"
                        else:
                             fullsql=fullsql+")')\n"

            print(fullsql)
            f.write(fullsql)

if __name__ == "__main__": 
    generator('insert.txt','asd','qwe',10000,4,0,1000,2)