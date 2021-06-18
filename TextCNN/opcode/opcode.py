import os
import time
import subprocess
import csv
from tqdm import tqdm


ida_path = 'C:\\Users\\bitcjx\\Desktop\\IDA\\ida.exe'
virus_dir = 'C:\\Users\\bitcjx\\Desktop\\dataset'
file_path = 'C:\\Users\\bitcjx\\Desktop\\res.csv'
with open(file_path, 'ab') as f:
	writer = csv.writer(f)
	writer.writerow(["name", "path", "opcode"])

for dir_name in os.listdir(virus_dir):
	
	if '-' not in dir_name:
		continue
	print 'start handle ' + dir_name
	dir_path = os.path.join(virus_dir, dir_name)
	for file in tqdm(os.listdir(dir_path)):

		file_path = os.path.join(dir_path, file)
		cmd = [ida_path, '-B', '-S"C:\\Users\\bitcjx\\Desktop\\IDA\\get_opcode_1.py"', file_path]
		a = subprocess.Popen(cmd)
		a.wait()                          