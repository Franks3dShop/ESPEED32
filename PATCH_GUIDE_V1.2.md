# 📦 Guide Simple - Appliquer le Patch TLE493D-W2B6 (Windows 11)

## 🆕 Version 1.2 - Mise à jour majeure

- ✅ Support des **4 variantes P3B6** (A0: 0x35, A1: 0x22, A2: 0x78, A3: 0x5D)
- ✅ **Documentation incluse** dans le patch (plus de copie manuelle)
- ✅ **8 capteurs supportés** au total (4× W2B6 + 4× P3B6)


## 🚀 Installation en 5 étapes

### 1️⃣ Créer un fork sur GitHub
1. Allez sur https://github.com/MakerFabio/ESPEED32
2. Cliquez sur **"Fork"** (en haut à droite)
3. Le fork sera sur votre compte : `https://github.com/VOTRE_USERNAME/ESPEED32`

---

### 2️⃣ Installer Git (si pas déjà fait)

#### Vérifier si Git est installé :
Ouvrez **PowerShell** ou **CMD** et tapez :
```cmd
git --version
```

#### Si Git n'est pas installé :
1. Téléchargez Git depuis : https://git-scm.com/download/win
2. Installez avec les options par défaut
3. Redémarrez PowerShell/CMD

---

### 3️⃣ Cloner votre fork

Ouvrez **PowerShell** (clic droit sur le menu Démarrer → Windows PowerShell)

```powershell
# Aller dans le dossier Documents
cd $HOME\Documents

# Cloner votre fork (remplacez VOTRE_USERNAME)
git clone https://github.com/VOTRE_USERNAME/ESPEED32.git

# Entrer dans le dossier
cd ESPEED32
```

---

### 4️⃣ Créer une branche et appliquer le patch

```powershell
# Créer une nouvelle branche
git checkout -b tle493d-w2b6-support

# Copier le patch depuis Téléchargements (ajustez le chemin si nécessaire)
Copy-Item "$HOME\Downloads\tle493d-w2b6-p3b6-support-v1.2.patch" -Destination .

# Appliquer le patch
git apply tle493d-w2b6-p3b6-support-v1.2.patch

# Vérifier les changements
git status
```

Vous devriez voir :
```
modified:   source/ESPEED32_V2_06/HAL.h
modified:   source/ESPEED32_V2_06/HAL.cpp
```

---

### 5️⃣ Committer et pousser

```powershell
# Ajouter les modifications
git add source/ESPEED32_V2_06/HAL.h source/ESPEED32_V2_06/HAL.cpp

# Créer le commit
git commit -m "Add TLE493D-W2B6 sensor support

- Support for all 4 W2B6 variants (A0, A1, A2, A3)
- Auto-detection of sensor variant
- Backward compatible with P3B6
- Resolves schematic/code mismatch"

# Pousser vers votre fork
git push origin tle493d-w2b6-support
```

---

## ✅ C'est fait !

Vos modifications sont maintenant sur votre fork GitHub.

---

## 📤 (Optionnel) Créer une Pull Request

Si vous voulez proposer vos modifications au projet original :

1. Allez sur votre fork : `https://github.com/VOTRE_USERNAME/ESPEED32`
2. Cliquez sur **"Compare & pull request"**
3. Remplissez la description
4. Cliquez **"Create pull request"**

---

## 🔧 Alternative : Utiliser Git Bash

Si PowerShell ne fonctionne pas bien, utilisez **Git Bash** (installé avec Git) :

1. Clic droit dans l'Explorateur → **Git Bash Here**
2. Utilisez les commandes Unix (comme sur Mac/Linux) :

```bash
cd ~/Documents
git clone https://github.com/VOTRE_USERNAME/ESPEED32.git
cd ESPEED32
git checkout -b tle493d-w2b6-support
cp ~/Downloads/tle493d-w2b6-p3b6-support-v1.2.patch .
git apply tle493d-w2b6-p3b6-support-v1.2.patch
git add source/ESPEED32_V2_06/HAL.h source/ESPEED32_V2_06/HAL.cpp
git commit -m "Add TLE493D-W2B6 sensor support"
git push origin tle493d-w2b6-support
```

---

## 🪟 Alternative : Utiliser GitHub Desktop (Interface Graphique)

### Méthode sans ligne de commande :

#### 1. Installer GitHub Desktop
- Téléchargez : https://desktop.github.com/
- Installez et connectez-vous avec votre compte GitHub

#### 2. Cloner votre fork
1. Ouvrez GitHub Desktop
2. **File** → **Clone repository**
3. Cherchez `VOTRE_USERNAME/ESPEED32`
4. Cliquez **Clone**

#### 3. Créer une branche
1. En haut : **Current Branch** → **New Branch**
2. Nom : `tle493d-w2b6-support`
3. Cliquez **Create Branch**

#### 4. Appliquer manuellement
Comme le patch ne peut pas être appliqué via l'interface, copiez les fichiers :

1. Téléchargez aussi **HAL.h** et **HAL.cpp**
2. Copiez-les dans : `Documents\ESPEED32\source\ESPEED32_V2_06\`
3. Remplacez les fichiers existants

#### 5. Committer et pousser
1. GitHub Desktop détectera les changements
2. En bas à gauche, écrivez le message de commit :
   ```
   Add TLE493D-W2B6 sensor support
   ```
3. Cliquez **Commit to tle493d-w2b6-support**
4. Cliquez **Push origin**

---

## 🔧 Si le patch ne s'applique pas

### Option A : Voir les erreurs
```powershell
git apply --check tle493d-w2b6-p3b6-support-v1.2.patch
```

Si des erreurs apparaissent, utilisez l'Option B.

### Option B : Copier les fichiers directement

Téléchargez aussi **HAL.h** et **HAL.cpp**, puis :

```powershell
# Copier les fichiers
Copy-Item "$HOME\Downloads\HAL.h" -Destination "source\ESPEED32_V2_06\"
Copy-Item "$HOME\Downloads\HAL.cpp" -Destination "source\ESPEED32_V2_06\"

# Committer
git add source\ESPEED32_V2_06\HAL.h source\ESPEED32_V2_06\HAL.cpp
git commit -m "Add TLE493D-W2B6 sensor support"
git push origin tle493d-w2b6-support
```

---

## 📋 Checklist Rapide

- [ ] Fork créé sur GitHub
- [ ] Git installé sur Windows
- [ ] Dépôt cloné localement
- [ ] Branche créée (`tle493d-w2b6-support`)
- [ ] Patch appliqué avec succès (ou fichiers copiés)
- [ ] Modifications committées
- [ ] Changements poussés vers votre fork
- [ ] (Optionnel) Pull Request créée

---

## 🎯 Résumé PowerShell (tout en une fois)

Après avoir créé le fork sur GitHub :

```powershell
# Cloner et préparer
cd $HOME\Documents
git clone https://github.com/VOTRE_USERNAME/ESPEED32.git
cd ESPEED32
git checkout -b tle493d-w2b6-support

# Appliquer le patch
Copy-Item "$HOME\Downloads\tle493d-w2b6-p3b6-support-v1.2.patch" -Destination .
git apply tle493d-w2b6-p3b6-support-v1.2.patch

# Committer et pousser
git add source/ESPEED32_V2_06/HAL.h source/ESPEED32_V2_06/HAL.cpp
git commit -m "Add TLE493D-W2B6 sensor support"
git push origin tle493d-w2b6-support
```

---

## 🎯 Résumé Git Bash (tout en une fois)

```bash
cd ~/Documents
git clone https://github.com/VOTRE_USERNAME/ESPEED32.git
cd ESPEED32
git checkout -b tle493d-w2b6-support
cp ~/Downloads/tle493d-w2b6-p3b6-support-v1.2.patch .
git apply tle493d-w2b6-p3b6-support-v1.2.patch
git add source/ESPEED32_V2_06/HAL.h source/ESPEED32_V2_06/HAL.cpp
git commit -m "Add TLE493D-W2B6 sensor support"
git push origin tle493d-w2b6-support
```

---

## ❓ Problèmes courants Windows

### Erreur : "git n'est pas reconnu comme commande"
**Solution** : 
1. Redémarrez PowerShell après l'installation de Git
2. Ou utilisez Git Bash à la place

### Erreur lors du push (demande mot de passe)
**Solution** :
1. Allez sur GitHub → Settings → Developer settings → Personal access tokens
2. Générez un nouveau token (classic)
3. Sélectionnez le scope "repo"
4. Copiez le token
5. Utilisez-le comme mot de passe quand Git demande

### Le chemin du patch ne fonctionne pas
**Solution** :
```powershell
# Vérifier où est le fichier
Get-ChildItem "$HOME\Downloads\*.patch"

# Ajuster le chemin si nécessaire
Copy-Item "C:\Users\VotreNom\Downloads\tle493d-w2b6-p3b6-support-v1.2.patch" -Destination .
```

### Problème avec les fins de ligne (CRLF vs LF)
**Solution** :
```powershell
# Configurer Git pour gérer automatiquement
git config --global core.autocrlf true
```

---

## 💡 Conseils Windows

### Utiliser Windows Terminal (recommandé)
Windows Terminal est plus moderne et pratique :
1. Installez depuis le Microsoft Store : **Windows Terminal**
2. Lancez PowerShell depuis Windows Terminal
3. Meilleure expérience de copier-coller

### Utiliser VS Code avec Git intégré
Si vous avez VS Code :
1. Ouvrez le dossier ESPEED32 dans VS Code
2. Utilisez l'onglet "Source Control" (Ctrl+Shift+G)
3. Interface visuelle pour Git (commit, push, etc.)

### Chemins dans Windows
- PowerShell : `$HOME\Documents` ou `C:\Users\VotreNom\Documents`
- Git Bash : `~/Documents` ou `/c/Users/VotreNom/Documents`
- Utilisez les guillemets si le chemin contient des espaces

---

## 🎓 Commandes utiles Windows

### PowerShell
```powershell
# Voir le contenu du dossier
Get-ChildItem
# ou
ls

# Afficher le chemin actuel
Get-Location
# ou
pwd

# Copier un fichier
Copy-Item source destination

# Éditer un fichier
notepad fichier.txt
```

### Git Bash (syntaxe Unix)
```bash
# Voir le contenu du dossier
ls

# Afficher le chemin actuel
pwd

# Copier un fichier
cp source destination

# Éditer un fichier
notepad fichier.txt
```

---

**Bonne chance avec Windows 11 ! 🪟🎉**

*Version: 1.1*  
*Compatible: Windows 10, Windows 11*  
*Date: 13 février 2026*
