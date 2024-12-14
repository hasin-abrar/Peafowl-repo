def get_value_from_prompt(file_name, prompt):
    with open(file_name) as f:
        lines = f.readlines()
    value = ''
    for line in lines:
        vals = line.strip().split(':')
        if vals[0] == prompt:
            # only for time infor
            prompt2 = "Elapsed (wall clock) time (h:mm:ss or m:ss):"
            line = line.strip()
            value = line[len(prompt2):]
            
    return value

def get_sec_from_time_str(time_str):
    vals = time_str.split(':')
    # print(vals)
    mult_list = [1, 60, 3600]
    total_time = 0
    ml_indx = 0
    for i in range(len(vals)-1, -1, -1):
        total_time  += float(vals[i])*mult_list[ml_indx]
        ml_indx += 1
    return total_time

def get_wall_clock_time_in_sec(file_name, prompt):
    wall_clock = get_value_from_prompt(file_name, prompt)
    total_time = get_sec_from_time_str(wall_clock)
    return total_time

def get_kmer_extraction_time_in_sec(genome_list, kmer_list, prompt):
    total_sec = 0
    for kmer in kmer_list:
        for genome in genome_list:
            fn = genome+"_"+str(kmer)+".jellyfish.rc.time"
            total_sec += get_wall_clock_time_in_sec(fn, prompt)
    return total_sec

def write_time_breakdown(out_fn, genome_list, kmer_list):
    prompt = "Elapsed (wall clock) time (h"
    kmer_extraction_time = get_kmer_extraction_time_in_sec(genome_list, kmer_list, prompt)
    entropy_fn = "time_for_entropy.txt"
    entropy_time = get_wall_clock_time_in_sec(entropy_fn, prompt)
    transpose_fn = "time_for_transpose.txt"
    transpose_time = get_wall_clock_time_in_sec(transpose_fn, prompt)
    raxml_fn = "time_for_raxml.txt"
    raxml_time = get_wall_clock_time_in_sec(raxml_fn, prompt)

    with open(out_fn, 'w') as f:
        f.write(str(round(kmer_extraction_time, 2))+' '+str(entropy_time - kmer_extraction_time)+' '+ str(transpose_time)+' '+str(raxml_time)+'\n')
    f.close()

def get_genome_list(fn):
    with open(fn) as f:
        lines = f.readlines()
    genome_list = [line.strip() for line in lines]
    return genome_list

def main():
    import sys
    genome_list_fn = sys.argv[1]
    out_fn = sys.argv[2]
    
    genome_list = get_genome_list(genome_list_fn)
    kmer_list = [i for i in range(9, 32, 2)]
    write_time_breakdown(out_fn, genome_list, kmer_list)

main()