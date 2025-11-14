import express from "express";
import crypto from "crypto";

const app = express();
const PORT = 3000;

function md5Hash(input) {
  return crypto.createHash('md5').update(input).digest('hex');
}

app.use(express.json());

// POST /drm route
app.post('/drm', (req, res) => {
    console.log('Received DRM request:', req.body);
    const jsonreq = req.body;
    const key = md5Hash(jsonreq.id);

    // Process the data here
    const result = {
        key:key
    };

    res.json(result);
});

// Start server
app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});