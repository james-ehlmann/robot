#!/usr/bin/env python
# -*- coding: utf-8 -*-
# curses_test.py
# Version: 1.0
# 
# This program must be used for good.
# This program comes with no warranty.
# 
# Originally written by a secular american named: Jay Ehlmann  
# To be reached at james(dot)ehlmann{at}gmail[dot]com (please don't email me.)
# 
# Originally developed for: Linux crunchbang 4.2.0-16-generic #19-Ubuntu SMP Thu Oct 8 15:35:06 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux

import os
import sys
import curses
import string
from json import loads, dumps as loads, dumps
import smbus


# file extension = .pollock

# D = done
# G = command received
# R = turn right 90
# L = turn left 90
# F = forward 1 tick
# B = backward 1 tick
# K = kill
# Vowels are colors to spray.
# A = green
# E = red
# U = blue
colors = 'AEU'

s = """
# terminal commands
# p = pause
# q = quit
# k = kill
# s = start / stop 
# r = reset screen size
# v = end voting
# n = next painting ready (in place.)
# [i| for i in string.digits] = add vote count to this painting
"""
s = s.split('\n')
for i in range(0, len(s)):
	scr.addstr(10 + i, 0, s[i])

addr = 0x04
bus = smbus.SMBUS(1)

def send_out(command):
	scr.addstr(4, 0, "Current command: " + command)
	scr.addstr(5, 0, 'recieving command')
	while(recieve() != 'G'):
		bus.write_byte(addr, ord(command))
		scr.addstr(5, 0, 'command recieved')
		if(scr.getch() == 'k'):
			pause = True # stop writing out, things are fucked
			
def recieve():
	return chr(bus.read_byte(addr)) # todo recieving logic.

# exit tech pollock
def leave():
	curses.nocbreak()
	scr.keypad(0)
	curses.echo()
	curses.endwin()
	exit(0)
	
rows = columns = 0
def reset_screen_size():
	global rows
	global columns
	rows, columns = os.popen('stty size', 'r').read().split()
	rows = int(rows)
	columns = int(columns)
	scr.addstr(rows - 1, 0, ":")
	curses.setsyx(rows, 1)
	scr.addstr(3, 0, str(rows) + ' ' + str(columns) )
	
scr = curses.initscr()
curses.cbreak()
scr.keypad(1)
scr.nodelay(1)
scr.addstr(1, 0, "Tech Pollock version 1")
reset_screen_size()

current_iter = '0'

def load_init_parent(relative_dir):
	tl = []
	for i in os.listdir(relative_dir):
		if(i.endswith(".pollock")):
			if(i.startswith("parent")):
				x = i.split('-')
				tl.append(int(x[1]))
	current_iter = str(tl.sort()[-1])
	
load_init_parent('.')
parent = loads(open("parent-"+ current_iter + "-.pollock", 'r').readlines())
pntgs = []

# one cell: [x, y, colors as string]

for i in range(0, 3):
	pntgs.append([])

max_size = 30
eight_size = max_size / 8
min_size = 1
max_y = 256
max_x = 256
eigth_y = max_y / 8
eigth_x = max_x / 8

current_pos = [max_x, max_y]
current_direction = 'f' # f = forward, r = right, l = left, b = backwards
current_pntg = 0
current_pntg_splatter = 0
current_command = ''
# max_y = far right
# max_y = far back


things_that_can_change = [0, 1, 2] # where 0 is the x, 1 is the y, 2 is the color 
# fifty fifty to change things
# if change, 1 / 3 change 1 value, 1 / 3 change 2 values, 1 / 3 change 3 values


def generate_children():
	for i in range(0, len(pntgs)):
		pntgs[i] = []
		for j in parent:
			if(random.randint(0, 1) == 1):
				x = random.sample(things_that_can_change, random.randint(1, 3))
				prep = j[:] # get our thing, w/out changing the parent
				for k in x:
					if k == 0: # 0 = x
						top = prep[0] + eigth_x
						bottom = prep[0] - eigth_x
						if(top > max_x):
							top = max_x
						elif(bottom < 0):
							bottom = 0
						prep[0] = random.randint(bottom, top) 
					elif k == 1:
						top = prep[1] + eigth_y
						bottom = prep[1] - eigth_y
						if(top > max_y):
							top = max_y
						elif(bottom < 0):
							bottom = 0
						prep[1] = random.randint(bottom, top)
					elif k == 2:
						colors_2 = colors.replace(prep[2], '') # too much of this, but what can you do?
						colors_2.remove(prep[2])
						prep[2] = colors_2[random.randint(0, len(colors_2))]
							
				pntgs[i].append(prep)
			else: # do no changes
				pntgs[i].append(j[:])
		change_in_size = random.randint(-1 * eight_size, eight_size) # bad way of donig things
		# on average wastes ~ 8th_size / 3 * len(pntgs)
		if(change_in_size > 0):
			counter = 0
			while(len(pntgs[i]) > min_size and counter < change_in_size):
				pntgs[i].append(gen_child())
				counter += 1
			
		elif(change_in_size < 0):
			counter = 0
			while(len(pntgs[i]) > min_size and counter < -1 * change_in_size):
				pntgs[i].pop()
				counter += 1
		else:
			# stay the same, do nothing.
			pass
		pntgs[i].sort() # after we put all the things in it we want order.
		
def gen_child():
	prep = []
	prep.append(random.randint(0, max_x))
	prep.append(random.randint(0, max_y))
	prep.append(colors[random.randint(0, len(colors))])
	return prep	

def set_parent(pntg):
	parent = []
	for i in pntg:
		parent.append(i[:])
	parent.sort()

start = False


start = False
pause = True
next_one = False
scr.addstr(3, 0, "status: not started")
votes = []
while(True):
	put = scr.getch()
	if(put < 256 and put > 0):
		put = chr(put)
		scr.addstr(2, 0, "this is the last terminal entry: '" + put + "'")
		if(put == 'q'):
			leave()
		elif(put == 'r'):
			reset_screen_size()
		elif(put in string.digits): # voting
			if(int(put) < len(votes)):
				votes[int(put)] += 1
		elif(put == 'v'): # calibrate votes, and generate next sequence
			voting = False
			f = open("parent-" + current_iter + "-.pollock", "w")
			f.write(dumps(pntgs[i]))
			for i in range(0, len(pntgs)):
				f = open("child-" + current_iter + "-" + i + "-.pollock", "w")
				f.write(dumps(pntgs[i]))
				f.close()
			winner = 0
			win_num = 0
			for i in range(0, len(votes)):
				if(votes[i] > win_num):
					win_num = votes[i]
					winner = i
			set_parent(pntgs[winner])
			generate_children()
			votes = []
			next_one = True
		elif(put == 's'): # start it
			# initialize communication
			start = not start
		elif(put == 'p'):
			pause = not pause
		elif(put == 'k'): # kill IT
			start = False
			send_out("K")
		elif(put == 'n'):
			next_one = False
			pause = True
	else:
		if(start): # initilize some stuff, so that our program can run effectively
			scr.addstr(3, 0, "status: starting         					")
			# get the last parent our program recognizez
			generate_children()
			start = not start
			next_one = True
		elif(pause): # pause the running of our program
			scr.addstr(3, 0, "status: paused						         ")
		elif(voting):
			scr.addstr(3, 0, "tallying votes" + str(votes))
		elif(next_one):
			scr.addstr(3, 0, "status: waiting for painting to be inserted.")
		else: # run our program
			recieved = recieve()
			if(recieved == 'D'):
				# need to update direction and things.
				splat = pntgs[current_pntg][current_pntg_splatter]
				if(current_pos[0] < splat[0]):
					if(current_direction == 'r'):
						send_out('F')
						current_pos[0] += 1
					elif(current_direction == 'l'):
						send_out('B')
						current_pos[0] += 1
					elif(current_direction == 'f'):
						send_out('R')
						current_direction = 'r'
					elif(current_direction == 'b'):
						send_out('L')
						current_direction = 'r'
				elif(current_pos[0] > splat[0]):
					if(current_direction == 'r'):
						send_out('B')
						current_pos[0] -= 1
					elif(current_direction == 'l'):
						send_out('F')
						current_pos[0] -= 1
					elif(current_direction == 'f'):
						send_out('L')
						current_direction = 'l'
					elif(current_direction == 'b'):
						send_out('R')
						current_direction = 'l'
				elif(current_pos[1] < splat[1]):
					if(current_direction == 'r'):
						send_out('L')
						current_direction = 'f'
					elif(current_direction == 'l'):
						send_out('R')
						current_direction = 'f'
					elif(current_direction == 'f'):
						send_out('B')
						current_pos[1] += 1
					elif(current_direction == 'b'):
						send_out('F')
						current_pos[1] += 1
				elif(current_pos[1] > splat[1]):
					if(current_direction == 'r'):
						send_out('L')
						current_direction = 'f'
					elif(current_direction == 'l'):
						send_out('R')
						current_direction = 'f'
					elif(current_direction == 'f'):
						send_out('F')
						current_pos[1] -= 1
					elif(current_direction == 'b'):
						send_out('B')
						current_pos[1] -= 1
				elif(current_direction != 'f'):
					if(current_direction == 'r'):
						send_out('L')
						current_direction = 'f'
					elif(current_direction == 'l'):
						send_out('R')
						current_direction = 'f'
					elif(current_direction == 'b'):
						send_out('R')
						current_direction = 'l'
				else:
					send_out(splat[2]) # send out our letter to paint with.
					current_pntg_splatter += 1
					if(current_pntg_splatter < len(pntgs[current_pntg])):
						pass # keep going in our loop
					else:
						current_pntg += 1 # we need to start on the next painting.
						current_pntg_splatter = 0
						if(current_pntg < len(pntgs)):
							if(current_pntg % 2):
								pntgs[current_pntg].reverse()
								next_one = True
							else: # because presumably we're at the front'
								 next_one = True
						else:
							current_iter = str(int(current_iter) + 1) # increment our current iter
							current_pntg = 0
							votes = [0 for x in range(0, len(pntgs))]
							voting = True # pause our shit, wait for painting
							
			elif(recieved == 'E'): # ERROR 0H SHIT
				# don't do anything until except maybe send the kill command
				pause = True				
			else:
				pass # maybe do some status updating in here, we don't have to do anything though'
			scr.addstr(3, 0, "status: running					")
		scr.refresh()
		

return 0

