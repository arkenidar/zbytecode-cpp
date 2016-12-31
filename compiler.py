#!/usr/bin/env python3

def line_type(line):
	if line.startswith('@'):
		return 'tag'
	elif line.startswith('#') or line=='':
		return 'comment'
	elif '=' in line:
		return 'copy'
	elif '|' in line:
		return 'jump2'
	else:
		return 'jump'

class TagSet():
	def __init__(self, mapping={}):
		self.mapping=mapping
		self.n=0
	def addTag(self,tag, n=None):
		if n != None:
			self.mapping[tag]=n
		elif tag not in self.mapping.keys():
			self.mapping[tag]=self.n
			self.n+=1
		return self.mapping[tag]

	def getTag(self, tag):
		return self.mapping[tag]

def parse_program(program):
	ln=-1
	tags=[]

	address_set=TagSet({'pch':255, 'in':254, 'out':254}); address=lambda tag:address_set.addTag(tag)
	line_set=TagSet(); line=lambda tag:line_set.addTag(tag)
	sets=(address_set,line_set)
	exe=[]
	for cur_line in program.splitlines():

		t=line_type(cur_line)
		if t=='comment': continue
		elif t=='copy':
			ln+=1
			for tag in tags: line_set.addTag(tag,ln)
			tags=[]
			copy_to,copy_from=cur_line.split('=')
			content=(copy_to,copy_from)
			for i in content: address(i)
			exe.append((ln,t,content))

		elif t=='jump2':
			ln+=1
			for tag in tags: line_set.addTag(tag,ln)
			tags=[]
			case1,case0=cur_line.split('|')
			content=(case1,case0)
			exe.append((ln,t,content))
			
		elif t=='jump':
			ln+=1
			for tag in tags: line_set.addTag(tag,ln)
			tags=[]
			target=cur_line
			content = target
			exe.append((ln,t,content))
			
		elif t=='tag':
			tag=cur_line.lstrip('@')
			tags.append(tag)
			content=tags
			exe.append((ln,t,content))
			
	return exe,sets
	
prog_not_gate='''
#not_gate.source
@start
pch=in
zero|one
@zero
out=0
start
@one
out=1
start
'''

prog_array_indexing='''
@start
x=in
i0=in
i1=in
pch=i0
sel1|sel0
@sel0
pch=i1
i10|i00
@sel1
pch=i1
i11|i01
@i00
a0=x
print
@i01
a1=x
print
@i10
a2=x
print
@i11
a3=x
print
@print
out=a0
out=a1
out=a2
out=a3
start
'''

def program_to_bytecode(program):
	parsed_program=parse_program(program)
	exe,sets=parsed_program
	bytecode=[]
	for i in exe:
		print(i)
		if i[1]=='copy':
			v=(i[2])[0]
			def v2(v):
				if v in ('0', '1'):
					return int(v)
				else:
					return sets[0].mapping[v]
			bytecode.append(0)
			bytecode.append(v2(v))
			v=(i[2])[1]
			bytecode.append(v2(v))
		elif i[1]=='jump':
			bytecode.append(1)
			v=i[2]
			bytecode.append(sets[1].mapping[v])
			bytecode.append(sets[1].mapping[v])
		elif i[1]=='jump2':
			bytecode.append(1)
			v=i[2]
			bytecode.append(sets[1].mapping[v[1]])
			bytecode.append(sets[1].mapping[v[0]])
	return bytecode

def n_to_hex(n):
	convert=('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F')
	low=convert[n&0x0F]
	high=convert[(n&0xF0)>>4]
	hexadecimal=high+low
	return hexadecimal

def bytecode_to_hex_string(bytecode):
	return ''.join(map(n_to_hex,bytecode))

bytecode=program_to_bytecode(prog_array_indexing)
print(bytecode_to_hex_string(bytecode))
