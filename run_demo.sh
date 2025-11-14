# Exit if any command fails
set -e

# === Configuration ===
URL="http://localhost:3000/drm"
DATA='{"id":"MYID"}' #MYID is replaced by a device specific identification token in real scenarios
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


#./encode
#./encryptor program.bin program.enc $KEY
./vm program.enc $KEY