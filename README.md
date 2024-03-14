This is a repo for my explorations with fractals, mostly Mandelbrot set generators. 
I decided to use this as a way to look at learning C vs Python, as well as making Python faster.  

Both the C and Python solutions require OpenMP to run the best version.  

On a mac this was achieved using homebrew: `brew install libomp llvm`

After installing llvm and libomp you will need to make sure to set your C compiler to llvm.

To build the C version:
1. from the c folder run: `mkdir build && cd build`
2. run: `cmake .. -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang -DCMAKE_C_FLAGS=-fopenmp` 
3. run: `make` 

To run the C version:
./fractal <size> <max_iterations>
where <size> is the height and width of the fractal image to be generated

To build the Python version:
1. Create a python virtual env: `python3 -m venv env` (or install `uv with curl -LsSf https://astral.sh/uv/install.sh | sh` and then run `uv venv env`)
2. Activate your env, `source env/bin/activate` 
3. Install the dependencies, either `pip install -r requirements.txt` or `uv pip install -r requirements.txt` depending on if you used uv above.  
4. set the CC environment variable: `export CC=/opt/homebrew/opt/llvm/bin/clang` 
5. Build the cython file: `python setup.py build_ext --inplace`

To run the Python version: 
  `python plot.py`
If you run `python plot.py -h` you can see all the options instead of just accepting the defaults

