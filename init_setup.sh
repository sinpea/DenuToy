# Exit if any command fails
set -e

gcc -o encode encode.c
gcc -o encryptor encryptor.c
gcc -o vm vm.c

# === Configuration ===
URL="http://localhost:3000/drm"
DATA='{"id":"MYID"}'
KEY="key"   # <-- The JSON field you want to extract

# === Make the POST request ===
# -s: silent mode (no progress)
# -X POST: send POST request
# -H: add HTTP header
# -d: send data
RESPONSE=$(curl -s -X POST "$URL" \
    -H "Content-Type: application/json" \
    -d "$DATA")

# === Extract value from JSON using jq ===
VALUE=$(echo "$RESPONSE" | jq -r ".$KEY")

# === Output ===
echo "Response: $RESPONSE"
echo "Extracted value for '$KEY': $VALUE"

./encode
./encryptor program.bin program.enc $KEY
rm program.bin