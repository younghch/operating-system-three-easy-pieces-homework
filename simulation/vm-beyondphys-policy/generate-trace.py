import sys
from os import system
from subprocess import run, PIPE
from random import randint, random

def run_randomly(count):
    return ','.join([str(randint(1, 10)) for _ in range(count)])

def run_locally(count, probability):
    last = randint(1, 10)
    trace = [last]
    for _ in range(count - 1):
        if random() < probability:
            move = 1
            if random() > 0.5: move = -1
            if  move == 1:
                cur = last%10 + move
            elif last == 1: 
                cur = 10
            else: 
                cur = last + move
            trace.append(cur)
        else:
            cur = randint(1, 10)
            trace.append(cur)
        last = cur
    return ','.join(map(str, trace))

def run_on_policies(policies, trace, clock_range = 1):
    for policy in policies:
        params = ['python3', 'paging-policy.py', '-c', '-C', '5', '-a', trace, '-p', policy]
        if policy == 'CLOCK':
            params.append('-b')
            for i in range(1, clock_range+1):
                params.append(str(i))
                print(f'clock bits : {i}')
                run_and_print(policy, params)
                params.pop()
        else:
            run_and_print(policy, params)

def run_and_print(policy, params):
        result = run(
            params,
            stdout = PIPE
        )
        idx = result.stdout.find(b'hits')
        result_string = result.stdout[idx:-2].decode('UTF-8')
        print(f'{policy}\n {result_string}')

args = sys.argv[1:]
policies = args[0].split(',')
if len(args) == 2:
    run_on_policies(policies, run_randomly(int(args[1])))
elif len(args) == 3:
    run_on_policies(policies, run_locally(int(args[1]), float(args[2])))
elif len(args) == 4:
    run_on_policies(policies, run_locally(int(args[1]), float(args[2])), int(args[3]))
else:
    print("please input two arguments: count, probability")

