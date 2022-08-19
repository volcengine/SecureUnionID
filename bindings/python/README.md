This is the python bindings for Secure ECC-based DID intersection. It currently only supports Linux and python 2.7.

How to build

for python3:
cd bindings/python
mkdir build
cd build
cmake ../
make

cd ../
python3 demo.py

for python2:
cd bindings/python
mkdir build
cd build
cmake -DPYTHON2=1 ../
make

cd ../
python2 demo.py


How to generate wrap file using swig?
for python3:
swig -python -py3 SecureUnionID.i
for pytthon2:
first comment out `define SWIG_PYTHON_STRICT_BYTE_CHAR`  in SecureUnionID.i, then
swig -python SecureUnionID.i

