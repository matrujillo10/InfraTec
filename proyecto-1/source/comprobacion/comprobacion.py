# coding: utf-8
original = open('../../data/caso2_Codificado.txt').read()
resultado = open('../../data/result.txt').read()
print 'Original: ', original
print 'Resultado:', resultado
print 'Son iguales' if resultado == original else 'No son iguales'
