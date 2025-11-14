# DenuToy

This is the toy version of a very popular DRM/AntiTamper suite - Denuvo. This was made as a demo for my 3rd semester presentation.

## How to Run:

First, start the server
```
cd drm_server
npm install
node index.js
```

If you are running it for the first time/made some changes in the binary using `encode.c` , run:
```
./init_setup.sh
```


Then, run:

```
./run_demo.sh
```

## Notes:
Still a Work In Progress. Contributions will be considered only after 1st December, 2025. The interpreter doesn't have a stack and only supports one print instruction.



