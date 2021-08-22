# komenda "python gen_long_comment.py" test z komentarzem o wielkości 5GB
# test sprawdza czy nie alokujemy niepotrzebnie buffora do wczytywania komentarzy

fn = "big_long_comment"

f = open(fn+".in", "w")
f.write("19\n19\n#")
for i in range(5):
	f.write("x"*1000000000)
f.write("\n19\n")
f.close()

f = open(fn+".out", "w")
f.write("1 2 4\n")
f.close()

f = open(fn+".err", "w")
f.close()

