
def divisao(b, a):
    if(a==0):
        print("Não se pode dividir por zero.\n")
        quit()
    
    quociente = 0
    resto = b

    while(True):
        resto = b - quociente*a
        
        if(resto < 0) : #Condição de parada
            resto += a
            quociente -= 1
            break
    
        quociente += 1

    return quociente, resto

def MDC(b, a):
    if(a == 0) : 
        return b
    
    else :
        q, r = divisao(b, a)
        return MDC(a, r)

b = 114
a = 27

q, r = divisao(b, a)

print(f"{MDC(126, 35)}\n")
