#!/usr/bin/env python3
from compiler import program_to_bytecode, prog_not_gate

def read_bit():
	while True:
		line=input('bit: ')
		options=('0','1')
		if line not in options:
			print('option not valid')
		else:
			break
	return int(line)

def run_bytecode():
	memory=[0 for _ in range(256)]
	memory[1]=1
	ip=0

	while ip!=255:
		t=bytecode[ip*3]

		if t==1:
			ip=bytecode[ip*3+1+memory[255]]
		elif t==0:
			din=bytecode[ip*3+2]
			if din==254: x=read_bit()
			else: x=memory[din]

			dout=bytecode[ip*3+1]
			if dout==254: print('out:',x)
			else: memory[dout]=x
			
			ip+=1

if __name__=='__main__':
	program=prog_not_gate
	bytecode=program_to_bytecode(program)
	run_bytecode()
