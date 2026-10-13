/**
 * XU VIS & Design System Interactive Core Controller
 * Features:
 * 1. Web Audio API Sonic Branding (Synthesizer Chimes & Feedback)
 * 2. Toast Feedback Notification System (Pure SVG Vector Icons)
 * 3. Dynamic Dual-Theme Switcher (Dark Obsidian / Light Snow Paper)
 * 4. Clipboard Color Swatch Copying
 * 5. Interactive Logo Sandbox Switcher
 * 6. XUFLOW Flowchart Code Review Simulator
 */

(function () {
  'use strict';

  // SVG Icon Vector Matrix (100% Vector, Zero Emojis)
  const SVG_ICONS = {
    bolt: `<svg class="xu-icon xu-icon-brand" viewBox="0 0 24 24"><polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"/></svg>`,
    music: `<svg class="xu-icon" viewBox="0 0 24 24"><path d="M9 18V5l12-2v13"/><circle cx="6" cy="18" r="3"/><circle cx="18" cy="16" r="3"/></svg>`,
    alert: `<svg class="xu-icon xu-icon-brand" viewBox="0 0 24 24"><path d="m21.73 18-8-14a2 2 0 0 0-3.48 0l-8 14A2 2 0 0 0 4 21h16a2 2 0 0 0 1.73-3Z"/><line x1="12" y1="9" x2="12" y2="13"/><line x1="12" y1="17" x2="12.01" y2="17"/></svg>`,
    copy: `<svg class="xu-icon" viewBox="0 0 24 24"><rect x="9" y="9" width="13" height="13" rx="2" ry="2"/><path d="M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1"/></svg>`,
    moon: `<svg class="xu-icon" viewBox="0 0 24 24"><path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"/></svg>`,
    sun: `<svg class="xu-icon xu-icon-gold" viewBox="0 0 24 24"><circle cx="12" cy="12" r="5"/><line x1="12" y1="1" x2="12" y2="3"/><line x1="12" y1="21" x2="12" y2="23"/><line x1="4.22" y1="4.22" x2="5.64" y2="5.64"/><line x1="18.36" y1="18.36" x2="19.78" y2="19.78"/><line x1="1" y1="12" x2="3" y2="12"/><line x1="21" y1="12" x2="23" y2="12"/><line x1="4.22" y1="19.78" x2="5.64" y2="18.36"/><line x1="18.36" y1="5.64" x2="19.78" y2="4.22"/></svg>`,
    soundOn: `<svg class="xu-icon" viewBox="0 0 24 24"><polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"/></svg>`,
    soundOff: `<svg class="xu-icon" viewBox="0 0 24 24"><polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><line x1="23" y1="9" x2="17" y2="15"/><line x1="17" y1="9" x2="23" y2="15"/></svg>`,
    palette: `<svg class="xu-icon" viewBox="0 0 24 24"><circle cx="13.5" cy="6.5" r=".5"/><circle cx="17.5" cy="10.5" r=".5"/><circle cx="8.5" cy="7.5" r=".5"/><circle cx="6.5" cy="12.5" r=".5"/><path d="M12 2C6.5 2 2 6.5 2 12s4.5 10 10 10c.926 0 1.648-.746 1.648-1.688 0-.437-.18-.835-.437-1.125-.29-.289-.438-.652-.438-1.125a1.64 1.64 0 0 1 1.668-1.668h1.996c3.051 0 5.555-2.503 5.555-5.554C21.965 6.012 17.461 2 12 2z"/></svg>`,
    eye: `<svg class="xu-icon" viewBox="0 0 24 24"><path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"/><circle cx="12" cy="12" r="3"/></svg>`,
    check: `<svg class="xu-icon xu-icon-green" viewBox="0 0 24 24"><polyline points="20 6 9 17 4 12"/></svg>`,
    flow: `<svg class="xu-icon xu-icon-cyan" viewBox="0 0 24 24"><polyline points="23 4 23 10 17 10"/><polyline points="1 20 1 14 7 14"/><path d="M3.51 9a9 9 0 0 1 14.85-3.36L23 10M1 14l4.64 4.36A9 9 0 0 0 20.49 15"/></svg>`,
    search: `<svg class="xu-icon" viewBox="0 0 24 24"><circle cx="11" cy="11" r="8"/><line x1="21" y1="21" x2="16.65" y2="16.65"/></svg>`,
    chat: `<svg class="xu-icon xu-icon-cyan" viewBox="0 0 24 24"><path d="M21 15a2 2 0 0 1-2 2H7l-4 4V5a2 2 0 0 1 2-2h14a2 2 0 0 1 2 2z"/></svg>`,
    rocket: `<svg class="xu-icon xu-icon-brand" viewBox="0 0 24 24"><path d="M4.5 16.5c-1.5 1.26-2 5-2 5s3.74-.5 5-2c.71-.84.7-2.13-.09-2.91a2.18 2.18 0 0 0-2.91-.09z"/><path d="m12 15-3-3a22 22 0 0 1 2-3.95A12.88 12.88 0 0 1 22 2c0 2.72-.78 7.5-6 11a22.35 22.35 0 0 1-4 2z"/><path d="M9 12H4s.55-3.03 2-4c1.62-1.08 5 0 5 0"/><path d="M12 15v5s3.03-.55 4-2c1.08-1.62 0-5 0-5"/></svg>`,
    sliders: `<svg class="xu-icon" viewBox="0 0 24 24"><line x1="4" y1="21" x2="4" y2="14"/><line x1="4" y1="10" x2="4" y2="3"/><line x1="12" y1="21" x2="12" y2="12"/><line x1="12" y1="8" x2="12" y2="3"/><line x1="20" y1="21" x2="20" y2="16"/><line x1="20" y1="12" x2="20" y2="3"/><line x1="1" y1="14" x2="7" y2="14"/><line x1="9" y1="8" x2="15" y2="8"/><line x1="17" y1="16" x2="23" y2="16"/></svg>`
  };

  function decorativeIcon(svg) {
    return svg.replace("<svg ", '<svg aria-hidden="true" focusable="false" ');
  }

  // State Management
  const state = {
    theme: (() => {
      try {
        return localStorage.getItem("xu-theme") || "dark";
      } catch (_) {
        return "dark";
      }
    })(),
    soundEnabled: true,
    audioCtx: null,
    activeTab: "vi-spec"
  };

  // Web Audio Context Initialization
  function getAudioContext() {
    if (!state.audioCtx) {
      const AudioCtxClass = window.AudioContext || window.webkitAudioContext;
      if (AudioCtxClass) {
        state.audioCtx = new AudioCtxClass();
      }
    }
    if (state.audioCtx && state.audioCtx.state === "suspended") {
      state.audioCtx.resume();
    }
    return state.audioCtx;
  }

  // Audio Synthesizer Helpers
  function playTone(freq, type = "sine", duration = 0.12, gainValue = 0.08) {
    if (!state.soundEnabled) return;
    const ctx = getAudioContext();
    if (!ctx) return;

    try {
      const osc = ctx.createOscillator();
      const gain = ctx.createGain();

      osc.type = type;
      osc.frequency.setValueAtTime(freq, ctx.currentTime);

      gain.gain.setValueAtTime(gainValue, ctx.currentTime);
      gain.gain.exponentialRampToValueAtTime(0.0001, ctx.currentTime + duration);

      osc.connect(gain);
      gain.connect(ctx.destination);

      osc.start();
      osc.stop(ctx.currentTime + duration);
    } catch (e) {
      console.warn("Audio playback error:", e);
    }
  }

  // Preset Brand Sounds
  function soundClick() {
    playTone(1800, "sine", 0.04, 0.04);
  }
  function soundStartupChime() {
    if (!state.soundEnabled) return;
    const ctx = getAudioContext();
    if (!ctx) return;
    const osc = state.audioCtx.createOscillator();
    const gain = state.audioCtx.createGain();
    osc.type = "sine";
    osc.frequency.setValueAtTime(440, state.audioCtx.currentTime);
    osc.frequency.exponentialRampToValueAtTime(880, state.audioCtx.currentTime + 0.35);
    gain.gain.setValueAtTime(0.1, state.audioCtx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, state.audioCtx.currentTime + 0.5);
    osc.connect(gain);
    gain.connect(state.audioCtx.destination);
    osc.start();
    osc.stop(state.audioCtx.currentTime + 0.5);
    showToast("正在播放: 品牌启动弦音 (Startup Chime 440→880Hz)", SVG_ICONS.music);
  }
  function soundBuildSuccess() {
    playTone(880, "sine", 0.09, 0.09);
    setTimeout(() => playTone(1320, "sine", 0.14, 0.09), 90);
    showToast("正在播放: 编译成功升调 (Build Success 880+1320Hz)", SVG_ICONS.bolt);
  }
  function soundDiagnosticAlert() {
    playTone(280, "sawtooth", 0.18, 0.09);
    showToast("正在播放: 错误诊断阻尼音 (Diagnostic Alert 280Hz)", SVG_ICONS.alert);
  }

  // Toast System (Pure SVG Vector Icons)
  function showToast(msg, iconSvg = SVG_ICONS.bolt) {
    const container = document.getElementById("toast-container");
    if (!container) return;
    const toast = document.createElement("div");
    toast.className = "toast";
    const rawIcon = (typeof iconSvg === "string" && iconSvg.startsWith("<svg")) ? iconSvg : (SVG_ICONS[iconSvg] || SVG_ICONS.bolt);
    const iconHtml = decorativeIcon(rawIcon);
    toast.innerHTML = `<span style="display:inline-flex; align-items:center;">${iconHtml}</span> <span>${msg}</span>`;
    container.appendChild(toast);
    setTimeout(() => {
      toast.style.opacity = "0";
      toast.style.transform = "translateY(10px)";
      toast.style.transition = "all 0.2s";
      setTimeout(() => toast.remove(), 200);
    }, 2800);
  }

  // Clipboard Helper
  function copyText(text, label) {
    if (navigator.clipboard) {
      navigator.clipboard.writeText(text).then(() => {
        showToast(`已复制 ${label || text} 到剪贴板`, SVG_ICONS.copy);
      }).catch(() => showToast("复制失败，请检查浏览器权限", SVG_ICONS.alert));
    } else {
      showToast(`复制成功: ${text}`, SVG_ICONS.copy);
    }
  }

  // Tab Navigation
  function initTabs() {
    const tabBtns = document.querySelectorAll(".nav-tab-btn[data-tab]");
    const panes = document.querySelectorAll(".tab-pane");

    tabBtns.forEach((btn) => btn.setAttribute("aria-current", btn.classList.contains("active") ? "page" : "false"));

    document.querySelectorAll("[data-tab-trigger]").forEach((trigger) => {
      trigger.addEventListener("click", () => {
        const target = document.querySelector(`[data-tab="${trigger.getAttribute("data-tab-trigger")}"]`);
        if (target) target.click();
      });
    });

    tabBtns.forEach((btn) => {
      btn.addEventListener("click", () => {
        soundClick();
        const targetTab = btn.getAttribute("data-tab");
        tabBtns.forEach((b) => b.classList.remove("active"));
        panes.forEach((p) => p.classList.remove("active"));

        btn.classList.add("active");
        const activePane = document.getElementById(targetTab);
        if (activePane) {
          activePane.classList.add("active");
        }
        tabBtns.forEach((b) => b.setAttribute("aria-current", b === btn ? "page" : "false"));
        state.activeTab = targetTab;
      });
    });
  }

  // Theme Toggle
  function applyTheme(theme) {
    state.theme = theme === "light" ? "light" : "dark";
    document.documentElement.setAttribute("data-theme", state.theme);
    const themeBtn = document.getElementById("theme-toggle-btn");
    if (themeBtn) {
      themeBtn.innerHTML = decorativeIcon(theme === "dark" ? SVG_ICONS.moon : SVG_ICONS.sun);
      themeBtn.setAttribute("title", theme === "dark" ? "切换至宣纸雪岩亮色模式" : "切换至曜石暗夜模式");
      themeBtn.setAttribute("aria-label", theme === "dark" ? "切换至亮色模式" : "切换至暗色模式");
      themeBtn.setAttribute("aria-pressed", theme === "light" ? "true" : "false");
    }
    try { localStorage.setItem("xu-theme", state.theme); } catch (_) { /* storage is optional */ }
    // 动态切换明暗模式对应的矢量资产 (黑红双色亮色版 / 霓虹反白暗夜版)
    document.querySelectorAll("img[data-light]").forEach((img) => {
      const targetSrc = theme === "dark" ? img.getAttribute("data-dark") : img.getAttribute("data-light");
      if (targetSrc) {
        img.src = targetSrc;
      }
    });
  }

  function initTheme() {
    const themeBtn = document.getElementById("theme-toggle-btn");
    if (!themeBtn) return;
    // 初始应用当前模式对应资产
    applyTheme(state.theme);
    themeBtn.addEventListener("click", () => {
      const nextTheme = state.theme === "dark" ? "light" : "dark";
      applyTheme(nextTheme);
      soundClick();
      showToast(`已切换至 ${nextTheme === "dark" ? "黑曜石暗夜" : "雪岩白 (经典黑红双色)"} 模式`, SVG_ICONS.palette);
    });
  }

  // Audio Toggle
  function initAudioToggle() {
    const audioBtn = document.getElementById("audio-toggle-btn");
    if (!audioBtn) return;
    audioBtn.addEventListener("click", () => {
      state.soundEnabled = !state.soundEnabled;
      audioBtn.innerHTML = decorativeIcon(state.soundEnabled ? SVG_ICONS.soundOn : SVG_ICONS.soundOff);
      audioBtn.setAttribute("aria-pressed", state.soundEnabled ? "true" : "false");
      if (state.soundEnabled) soundClick();
      showToast(`音效已${state.soundEnabled ? "开启" : "静音"}`, SVG_ICONS.music);
    });
  }

  // Color Swatch Copy Listener
  function initColorSwatches() {
    document.querySelectorAll(".color-swatch-card").forEach((card) => {
      card.addEventListener("click", () => {
        const hex = card.getAttribute("data-hex");
        const name = card.querySelector(".swatch-name").textContent;
        copyText(hex, `${name} (${hex})`);
      });
    });
  }

  // Logo Sandbox Mode Switcher
  function initLogoModeSwitcher() {
    const stage = document.getElementById("logo-mode-stage");
    const img = document.getElementById("logo-mode-img");
    const btns = document.querySelectorAll(".logo-mode-btn");

    if (!stage || !img) return;

    btns.forEach((btn) => {
      btn.setAttribute("aria-pressed", btn.classList.contains("active") ? "true" : "false");
      btn.addEventListener("click", () => {
        soundClick();
        btns.forEach((b) => b.classList.remove("active"));
        btn.classList.add("active");
        btns.forEach((b) => b.setAttribute("aria-pressed", b === btn ? "true" : "false"));
        const mode = btn.getAttribute("data-mode");

        if (mode === "dark") {
          stage.style.background = "radial-gradient(circle, #1A0912 0%, #050508 100%)";
          img.src = "brand/xu_classic.svg";
        } else if (mode === "white") {
          stage.style.background = "#050508";
          img.src = "brand/xu_logo_monochrome_white.svg";
        } else if (mode === "black") {
          stage.style.background = "#FFFFFF";
          img.src = "brand/xu_logo_monochrome_black.svg";
        } else if (mode === "light") {
          stage.style.background = "#FAFAF9";
          img.src = "brand/xu_classic.svg";
        }
        showToast(`已切换为标志模式: ${btn.textContent.trim()}`, SVG_ICONS.eye);
      });
    });
  }

  // Sonic Triggers
  function initSonicTriggers() {
    const btnChime = document.getElementById("btn-play-chime");
    const btnSuccess = document.getElementById("btn-play-success");
    const btnAlert = document.getElementById("btn-play-alert");

    if (btnChime) btnChime.addEventListener("click", soundStartupChime);
    if (btnSuccess) btnSuccess.addEventListener("click", soundBuildSuccess);
    if (btnAlert) btnAlert.addEventListener("click", soundDiagnosticAlert);
  }

  // XUFLOW Flowchart Review Interactive Simulation
  function initFlowReviewSimulator() {
    const approveBtn = document.getElementById("btn-approve-flow");
    const commentBtn = document.getElementById("btn-comment-flow");
    const modeOwnership = document.getElementById("mode-ownership-flow");
    const modeConcurrency = document.getElementById("mode-concurrency-flow");
    const modeError = document.getElementById("mode-error-flow");

    if (approveBtn) {
      approveBtn.addEventListener("click", () => {
        soundBuildSuccess();
        showToast("PR #1024 流程图审查通过：已标记为 Approved", SVG_ICONS.check);
      });
    }

    if (commentBtn) {
      commentBtn.addEventListener("click", () => {
        window.addReviewComment();
      });
    }

    const modeBtns = [modeOwnership, modeConcurrency, modeError];
    modeBtns.forEach(btn => {
      if (!btn) return;
      btn.addEventListener("click", () => {
        modeBtns.forEach(b => b && b.classList.remove("active"));
        btn.classList.add("active");
        soundClick();
        showToast(`已切换至审查视图: ${btn.textContent.trim()}`, SVG_ICONS.flow);
      });
    });
  }

  // Global functions for Flowchart node clicks and comments
  window.selectFlowNode = function(nodeIndex) {
    soundClick();
    document.querySelectorAll(".flow-node-card").forEach((el, idx) => {
      if (idx + 1 === nodeIndex) {
        el.classList.add("active");
      } else {
        el.classList.remove("active");
      }
    });
    showToast(`已定位流程图审查节点 ${nodeIndex}`, SVG_ICONS.search);
  };

  window.addReviewComment = function() {
    soundClick();
    const comment = prompt("请输入针对当前流程图节点的审查批注:", "此节点所有权生命周期闭环，满足零拷贝并发安全。");
    if (comment) {
      showToast("批注已成功提交至 XUFLOW Review 讨论区", SVG_ICONS.chat);
    }
  };

  // Interactive Playground Simulation
  function initPlayground() {
    const playRunBtn = document.getElementById("play-run-btn");
    const playOutput = document.getElementById("play-output");
    const playBench = document.getElementById("play-bench-time");

    if (!playRunBtn || !playOutput) return;

    playRunBtn.addEventListener("click", () => {
      soundClick();
      playRunBtn.classList.add("btn-loading");
      playOutput.textContent = "Compiling and executing in sandbox...";

      const t0 = performance.now();
      setTimeout(() => {
        playRunBtn.classList.remove("btn-loading");
        const ms = (performance.now() - t0).toFixed(2);
        playOutput.textContent = `[Compiled in 10.4ms]
[Execution output]:
Area of circle: 314.159
Processed 2 items safely
No GC pauses · Memory usage: 1.2 MB
Exit code: 0`;
        if (playBench) {
          playBench.textContent = `${ms} ms`;
        }
        soundBuildSuccess();
        showToast(`Playground 运行成功 (${ms}ms)`, SVG_ICONS.rocket);
      }, 200);
    });
  }

  // Toggles Demo
  function initToggles() {
    document.querySelectorAll(".xu-toggle").forEach((toggle) => {
      toggle.parentElement.addEventListener("click", () => {
        toggle.classList.toggle("checked");
        soundClick();
        showToast(`状态已切换: ${toggle.classList.contains("checked") ? "开启" : "关闭"}`, SVG_ICONS.sliders);
      });
    });
  }

  function initToastDemos() {
    document.querySelectorAll("[data-toast]").forEach((button) => {
      button.addEventListener("click", () => {
        soundClick();
        showToast(button.getAttribute("data-toast"), button.getAttribute("data-toast-icon") || "bolt");
      });
    });
  }

  function initIconAccessibility() {
    document.querySelectorAll("svg.xu-icon:not([role])").forEach((icon) => {
      icon.setAttribute("aria-hidden", "true");
      icon.setAttribute("focusable", "false");
    });
  }

  // Initialize all modules on DOM ready
  document.addEventListener("DOMContentLoaded", () => {
    initIconAccessibility();
    initTabs();
    initTheme();
    initAudioToggle();
    initColorSwatches();
    initLogoModeSwitcher();
    initSonicTriggers();
    initFlowReviewSimulator();
    initPlayground();
    initToggles();
    initToastDemos();

    // Global copy button handler
    document.querySelectorAll("[data-copy]").forEach((btn) => {
      btn.addEventListener("click", (e) => {
        e.stopPropagation();
        const targetId = btn.getAttribute("data-copy");
        const elem = document.getElementById(targetId);
        if (elem) {
          copyText(elem.textContent, "内容");
        }
      });
    });
  });
})();
