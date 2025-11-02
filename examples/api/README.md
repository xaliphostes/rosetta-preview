# Example using IDL file

## 1. Make your rosetta-gen.py executable

```sh
chmod +x rosetta-gen.py
```

## 2. Go to the example/api folder

## 3. Create a Python env

```sh
python -m venv .venv
source .venv/bin/activate
```

## 4. Install yaml

```sh
pip install pyyaml
```

## 5. Generate the bindings

```sh
../../rosetta-gen.py API.yaml --output generated
```

## 6. Build

Go to the `generated` directory, then

### Configure
```sh
cmake -S . -B build -DCMAKE_MODULE_PATH="$(pwd)/../../../cmake" -DBUILD_NODE_ADDON=OFF
```

### Build
```sh
cmake --build build
```
