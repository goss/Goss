a = 6
b = 9
c = a + b
d = "Lua String Test"

player = {Name = "Goss", Level = 99}

function LuaFunc(r1, r2)
	io.write("[lua] LuaFunc("..r1..", "..r2..")\n")
	r = HostFunction(r1, r2)
	io.write("[lua] HostFunction returned "..r.."\n")
	return r * 15
end