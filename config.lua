--
-- COMP3214 Coursework 3 configuration
--
-- Name: Reuben Ng (rdcn1g14)
--
print("Config.lua executed")
GLFW = 3 -- use 2 (GLFW 2), 3 (glfw 3) or 1 (freeglut, not tested yet)
GLew = true -- must use this.
IMAGE = 'stb_image' -- not using stb
STD = 'c++11'
BOOST = false -- nil, false or true.
BULLET = false -- nil, false or true.
MODEL = false -- nil, false or true.
GUI = 'ImGui' -- ImGui or  nil
FILES = nil -- nil or list of files. nil is equivalent to {'.cpp'}.