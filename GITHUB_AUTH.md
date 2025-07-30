# 🔐 GitHub Authentication Guide

## Current Issue: Git Authentication Required

Git is asking for GitHub credentials to push your code. Here are the solutions:

## ✅ **Option 1: Personal Access Token (Easiest)**

### Step 1: Create Personal Access Token
1. Go to https://github.com/settings/tokens
2. Click "Generate new token" → "Generate new token (classic)"
3. Note: `embedded-hal-project-upload`
4. Expiration: `90 days` (or your preference)
5. Select scopes:
   - ✅ `repo` (Full control of private repositories)
   - ✅ `workflow` (Update GitHub Action workflows)
6. Click "Generate token"
7. **COPY THE TOKEN** (you won't see it again!)

### Step 2: Use Token for Authentication
When Git asks for credentials:
- **Username**: `ChandrashekarCYoga`
- **Password**: `<paste your personal access token>`

## ✅ **Option 2: SSH Authentication (More Secure)**

### Step 1: Generate SSH Key
```bash
ssh-keygen -t ed25519 -C "your.email@example.com"
# Press Enter for default location
# Set a passphrase (optional)
```

### Step 2: Add SSH Key to GitHub
```bash
# Copy your public key
cat ~/.ssh/id_ed25519.pub
```
1. Go to https://github.com/settings/keys
2. Click "New SSH key"
3. Title: `Embedded HAL Development`
4. Paste the public key content
5. Click "Add SSH key"

### Step 3: Change Remote to SSH
```bash
git remote set-url origin git@github.com:ChandrashekarCYoga/embedded-hal-temperature-controller.git
git push -u origin main
```

## ✅ **Option 3: GitHub CLI (Alternative)**

### Install and Authenticate
```bash
# Install GitHub CLI (if available)
sudo apt install gh  # or your package manager

# Authenticate
gh auth login
# Choose: GitHub.com → HTTPS → Login with web browser
```

## 🚨 **Quick Solution (If Stuck Now)**

If the terminal is stuck asking for credentials:

1. **Press Ctrl+C** to cancel the current operation
2. Choose one of the authentication methods above
3. Try the push again:
   ```bash
   git push -u origin main
   ```

## 📋 **Recommended Approach**

For this project upload, I recommend **Personal Access Token**:

1. Create token at https://github.com/settings/tokens
2. Copy the token
3. Run: `git push -u origin main`
4. When prompted:
   - Username: `ChandrashekarCYoga`
   - Password: `<your-token>`

## ⚠️ **Security Notes**

- **Personal Access Token**: Treat like a password, don't share
- **SSH Key**: More secure, better for regular development
- **Never commit tokens/keys** to your repository

## 🔄 **After Authentication**

Once authenticated successfully, your repository will be available at:
**https://github.com/ChandrashekarCYoga/embedded-hal-temperature-controller**

---

**Choose the method you're most comfortable with!** 🚀
