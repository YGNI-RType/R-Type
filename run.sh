mkdir -p build
cd build
touch .gitkeep
cmake .. -DCMAKE_TOOLCHAIN_FILE=./cmake/define-compilers.cmake || { echo "CMake configuration failed"; exit 1; }
cmake --build . || { echo "CMake build failed"; exit 1; }

if [[ "$(uname)" == "Darwin" ]]; then
    echo "Running on macOS. Copying GEngine..."
    mkdir -p ../build_includes/
    cp -rf ../build/.vcpkg/installed/arm64-osx/include/ ../build_includes/
fi

echo "Running server..."
mkfifo pipe
./r-type_server > pipe &
SERVER_PID=$!
tee server_output.log < pipe &
TEE_PID=$!

cleanup() {
    echo "\nCleaning up..."
    kill $SERVER_PID 2>/dev/null
    kill $TEE_PID 2>/dev/null
    rm pipe
    kill $CLIENT_PID 2>/dev/null
    exit 0
}

# Trap CTRL + C (SIGINT)
trap cleanup SIGINT

sleep 1

echo "Running client..."
./r-type_client

kill $SERVER_PID 2>/dev/null
kill $TEE_PID 2>/dev/null
rm pipe

cd ..
exit 0
