/**
 * Tone - Tactile Desktop Sound Controller
 * Interactive Web Simulator & Documentation Engine
 * (c) 2026 AtaCanYmc & Sezin Sözden
 */

// ==========================================================================
// Sound Modes Configuration (Matching ToneOS Firmware ToneController.cpp)
// ==========================================================================
const MODES = [
  {
    index: 0,
    name: "Volume",
    label: "Volume",
    minValue: 0,
    maxValue: 100,
    currentValue: 65,
    r: 255,
    g: 255,
    b: 155,
    colorHex: "#fffdf0",
    glowRgba: "rgba(255, 253, 240, 0.4)",
    freqRange: "Full Range (Master Gain)"
  },
  {
    index: 1,
    name: "Bass",
    label: "Bass",
    minValue: 0,
    maxValue: 100,
    currentValue: 45,
    r: 122,
    g: 50,
    b: 245,
    colorHex: "#8b5cf6",
    glowRgba: "rgba(139, 92, 246, 0.45)",
    freqRange: "32 Hz – 250 Hz (Low End)"
  },
  {
    index: 2,
    name: "Treble",
    label: "Treble",
    minValue: 0,
    maxValue: 100,
    currentValue: 55,
    r: 90,
    g: 240,
    b: 255,
    colorHex: "#06b6d4",
    glowRgba: "rgba(6, 182, 212, 0.45)",
    freqRange: "4 kHz – 16 kHz (High End)"
  }
];

const NUM_PIXELS = 11;
let currentModeIndex = 0;
let isAudioMuted = false;
let audioCtx = null;

// ==========================================================================
// Web Audio API Synthesizer for Tactile Feedback
// ==========================================================================
function playHapticTone(freq = 440, type = "sine", duration = 0.05) {
  if (isAudioMuted) return;
  try {
    if (!audioCtx) {
      audioCtx = new (window.AudioContext || window.webkitAudioContext)();
    }
    if (audioCtx.state === "suspended") {
      audioCtx.resume();
    }
    const osc = audioCtx.createOscillator();
    const gain = audioCtx.createGain();
    osc.type = type;
    osc.frequency.setValueAtTime(freq, audioCtx.currentTime);
    gain.gain.setValueAtTime(0.08, audioCtx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, audioCtx.currentTime + duration);
    osc.connect(gain);
    gain.connect(audioCtx.destination);
    osc.start();
    osc.stop(audioCtx.currentTime + duration);
  } catch (e) {
    // Graceful fallback if Web Audio is restricted
  }
}

// ==========================================================================
// Interactive Simulator Engine
// ==========================================================================
class ToneSimulator {
  constructor() {
    this.stage = document.getElementById("deviceStage");
    this.dial = document.getElementById("tactileDial");
    this.centerBtn = document.getElementById("centerSphereBtn");
    this.sphereModeVal = document.getElementById("sphereModeVal");
    this.ledArcSvg = document.getElementById("ledArcSvg");
    this.telMode = document.getElementById("telMode");
    this.telVal = document.getElementById("telVal");
    this.telAngle = document.getElementById("telAngle");
    this.bleStream = document.getElementById("bleStream");

    this.isDragging = false;
    this.currentAngle = 180;
    this.initLedArc();
    this.attachEventListeners();
    this.render();
  }

  initLedArc() {
    if (!this.ledArcSvg) return;
    this.ledArcSvg.innerHTML = "";

    const cx = 180;
    const cy = 180;
    const radius = 150;
    const startAngle = 135; // degrees
    const endAngle = 405;   // 270 degree sweep

    // Render 11 LED Nodes
    for (let i = 0; i < NUM_PIXELS; i++) {
      const angleDeg = startAngle + (i / (NUM_PIXELS - 1)) * (endAngle - startAngle);
      const angleRad = (angleDeg * Math.PI) / 180;
      const x = cx + radius * Math.cos(angleRad);
      const y = cy + radius * Math.sin(angleRad);

      const circle = document.createElementNS("http://www.w3.org/2000/svg", "circle");
      circle.setAttribute("cx", x);
      circle.setAttribute("cy", y);
      circle.setAttribute("r", "7");
      circle.setAttribute("fill", "#1e293b");
      circle.setAttribute("id", `ledNode_${i}`);
      circle.setAttribute("class", "led-node");
      this.ledArcSvg.appendChild(circle);
    }
  }

  attachEventListeners() {
    // Tap Center Button -> Cycle Mode
    this.centerBtn.addEventListener("click", (e) => {
      e.stopPropagation();
      this.nextMode();
    });

    // Mouse / Touch Drag Rotation on Dial
    const startDrag = (e) => {
      this.isDragging = true;
      this.updateFromPointer(e);
    };

    const doDrag = (e) => {
      if (!this.isDragging) return;
      this.updateFromPointer(e);
    };

    const stopDrag = () => {
      this.isDragging = false;
    };

    this.dial.addEventListener("mousedown", startDrag);
    window.addEventListener("mousemove", doDrag);
    window.addEventListener("mouseup", stopDrag);

    this.dial.addEventListener("touchstart", startDrag, { passive: false });
    window.addEventListener("touchmove", doDrag, { passive: false });
    window.addEventListener("touchend", stopDrag);

    // Mouse Wheel on Dial
    this.dial.addEventListener("wheel", (e) => {
      e.preventDefault();
      const delta = e.deltaY > 0 ? -2 : 2;
      this.adjustValue(delta);
    }, { passive: false });

    // Keyboard Navigation
    this.dial.setAttribute("tabindex", "0");
    this.dial.setAttribute("role", "slider");
    this.dial.setAttribute("aria-label", "Tone Dial Adjuster");
    this.dial.addEventListener("keydown", (e) => {
      if (e.key === "ArrowUp" || e.key === "ArrowRight") {
        e.preventDefault();
        this.adjustValue(2);
      } else if (e.key === "ArrowDown" || e.key === "ArrowLeft") {
        e.preventDefault();
        this.adjustValue(-2);
      } else if (e.key === " " || e.key === "Enter") {
        e.preventDefault();
        this.nextMode();
      }
    });
  }

  updateFromPointer(e) {
    if (e.cancelable && e.type.startsWith("touch")) {
      e.preventDefault();
    }
    const rect = this.dial.getBoundingClientRect();
    const cx = rect.left + rect.width / 2;
    const cy = rect.top + rect.height / 2;
    const clientX = e.touches ? e.touches[0].clientX : e.clientX;
    const clientY = e.touches ? e.touches[0].clientY : e.clientY;

    const dx = clientX - cx;
    const dy = clientY - cy;
    let deg = (Math.atan2(dy, dx) * 180) / Math.PI + 90;
    if (deg < 0) deg += 360;

    // Constrain to active 270 deg sweep (45 deg to 315 deg)
    const sweepStart = 45;
    const sweepEnd = 315;
    let clampedDeg = Math.max(sweepStart, Math.min(sweepEnd, deg));
    const percent = Math.round(((clampedDeg - sweepStart) / (sweepEnd - sweepStart)) * 100);

    const activeMode = MODES[currentModeIndex];
    if (activeMode.currentValue !== percent) {
      activeMode.currentValue = percent;
      this.currentAngle = clampedDeg - 180;
      playHapticTone(220 + percent * 4, "sine", 0.02);
      this.render();
    }
  }

  adjustValue(delta) {
    const activeMode = MODES[currentModeIndex];
    const prev = activeMode.currentValue;
    activeMode.currentValue = Math.max(0, Math.min(100, activeMode.currentValue + delta));
    if (prev !== activeMode.currentValue) {
      this.currentAngle = (activeMode.currentValue / 100) * 270 - 135;
      playHapticTone(240 + activeMode.currentValue * 3, "sine", 0.02);
      this.render();
    }
  }

  nextMode() {
    currentModeIndex = (currentModeIndex + 1) % MODES.length;
    const mode = MODES[currentModeIndex];

    // Update CSS custom property tokens
    document.documentElement.style.setProperty("--accent-active", mode.colorHex);
    document.documentElement.style.setProperty("--accent-glow", mode.glowRgba);

    playHapticTone(520, "triangle", 0.08);
    this.render();
  }

  render() {
    const mode = MODES[currentModeIndex];

    // Rotate Dial Element
    const dialRotation = (mode.currentValue / 100) * 270 - 135;
    this.dial.style.transform = `rotate(${dialRotation}deg)`;

    // Center Sphere Label
    this.sphereModeVal.textContent = mode.name;
    this.sphereModeVal.style.color = mode.colorHex;

    // NeoPixel LED Arc Logic (Matching PixelController.cpp)
    const activeLedCount = Math.round((mode.currentValue / 100) * NUM_PIXELS);
    for (let i = 0; i < NUM_PIXELS; i++) {
      const node = document.getElementById(`ledNode_${i}`);
      if (node) {
        if (i < activeLedCount) {
          node.setAttribute("fill", mode.colorHex);
          node.style.filter = `drop-shadow(0 0 8px ${mode.colorHex})`;
        } else {
          node.setAttribute("fill", "#1e293b");
          node.style.filter = "none";
        }
      }
    }

    // Update Telemetry Displays
    if (this.telMode) this.telMode.textContent = mode.name;
    if (this.telVal) this.telVal.textContent = `${mode.currentValue}%`;
    if (this.telAngle) this.telAngle.textContent = `${Math.round(dialRotation + 135)}°`;

    // Emit Simulated BLE Notification JSON
    const blePayload = {
      mode: mode.name,
      value: String(mode.currentValue),
      r: String(mode.r),
      g: String(mode.g),
      b: String(mode.b)
    };
    if (this.bleStream) {
      this.bleStream.textContent = JSON.stringify(blePayload, null, 2);
    }
  }
}

// ==========================================================================
// Documentation Tabbed Browser & Pinout Switcher Setup
// ==========================================================================
function initDocTabs() {
  const tabs = document.querySelectorAll(".docs-tab-btn");
  const articles = document.querySelectorAll(".docs-article");

  tabs.forEach(tab => {
    tab.addEventListener("click", () => {
      const targetId = tab.getAttribute("data-target");

      tabs.forEach(t => t.classList.remove("active"));
      articles.forEach(a => a.classList.remove("active"));

      tab.classList.add("active");
      const targetArticle = document.getElementById(targetId);
      if (targetArticle) {
        targetArticle.classList.add("active");
      }
    });
  });
}

function initPinoutSwitcher() {
  const pinoutTabs = document.querySelectorAll(".pinout-tab-btn");
  const pinoutRows = document.querySelectorAll(".pinout-row");

  pinoutTabs.forEach(tab => {
    tab.addEventListener("click", () => {
      const targetBoard = tab.getAttribute("data-board");
      pinoutTabs.forEach(t => t.classList.remove("active"));
      tab.classList.add("active");

      pinoutRows.forEach(row => {
        const pinCell = row.querySelector(".target-pin");
        if (pinCell) {
          pinCell.textContent = row.getAttribute(`data-${targetBoard}`);
        }
      });
    });
  });
}

function initCopyButtons() {
  const copyButtons = document.querySelectorAll(".copy-btn");
  copyButtons.forEach(btn => {
    btn.addEventListener("click", () => {
      const targetCodeId = btn.getAttribute("data-code-target");
      const codeBlock = document.getElementById(targetCodeId);
      if (codeBlock) {
        navigator.clipboard.writeText(codeBlock.textContent.trim()).then(() => {
          const originalText = btn.textContent;
          btn.textContent = "Copied!";
          setTimeout(() => {
            btn.textContent = originalText;
          }, 2000);
        });
      }
    });
  });
}

// ==========================================================================
// DOM Initialization
// ==========================================================================
document.addEventListener("DOMContentLoaded", () => {
  new ToneSimulator();
  initDocTabs();
  initPinoutSwitcher();
  initCopyButtons();
});
