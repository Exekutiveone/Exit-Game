startOrResumeTimer(); // 20 Minuten

function handleLogin() {
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;
  
    // ✅ Hier kannst du die Login-Daten anpassen
    const correctUsername = "Luc Moreau";
    const correctPassword = "Cobalt42";
  
    if (username === correctUsername && password === correctPassword) {
      window.location.href = "index4_1.html";
    } else {
      alert("Falsche Zugangsdaten!");
    }
  }