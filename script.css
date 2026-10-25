const $ = (id) => document.getElementById(id);
const EXAMPLE = [82, 170, 43, 140, 24, 16, 190];
let simulation = null;
let step = 0;
let timer = null;
let chart = null;
let headPosition = 50;
let diskLimit = 200;
let animationFrame = null;
let animationProgress = 1;
let previousHead = 50;

function parseInput() {
  const raw = $('requestsInput').value.trim();
  if (!raw) throw new Error('Enter at least one cylinder request.');
  const values = raw.split(/[\s,;]+/).filter(Boolean).map(Number);
  if (values.some(n => !Number.isInteger(n))) throw new Error('Requests must be whole numbers separated by commas.');
  diskLimit = Number($('diskSize').value);
  if (values.some(n => n < 0 || n >= diskLimit)) throw new Error(`Every request must be between 0 and ${diskLimit - 1}.`);
  const initial = Number($('headInput').value);
  if (!Number.isInteger(initial) || initial < 0 || initial >= diskLimit) throw new Error(`Initial head must be between 0 and ${diskLimit - 1}.`);
  return { requests: values, initial };
}
function buildSimulation() {
  stopPlayback();
  try {
    const { requests, initial } = parseInput();
    simulation = { requests, initial, positions: [initial, ...requests], moves: [] };
    let from = initial;
    requests.forEach((to, i) => {
      simulation.moves.push({ index: i + 1, from, to, distance: Math.abs(to - from) });
      from = to;
    });
    step = 0; headPosition = initial; previousHead = initial; animationProgress = 1;
    $('errorBox').style.display = 'none';
    $('requestTotal').textContent = `/ ${requests.length} requests`;
    updateChart(); renderAll(); drawDisk();
  } catch (err) {
    $('errorBox').textContent = err.message;
    $('errorBox').style.display = 'block';
  }
}
function currentTotal() { return simulation.moves.slice(0, step).reduce((sum, m) => sum + m.distance, 0); }
function renderAll() {
  if (!simulation) return;
  const total = currentTotal();
  headPosition = simulation.positions[step];
  $('totalMovement').textContent = total;
  $('avgMovement').textContent = step ? (total / step).toFixed(2) : '0.00';
  $('servedCount').textContent = step;
  $('currentHead').textContent = headPosition;
  $('progressBar').style.width = `${step / simulation.requests.length * 100}%`;
  $('stepCounter').textContent = `${step} / ${simulation.requests.length}`;
  $('canvasHeadLabel').textContent = `HEAD @ ${headPosition}`;
  $('statusText').textContent = step === simulation.requests.length ? 'All requests completed' : step === 0 ? 'Ready to simulate' : `Serving request ${step} of ${simulation.requests.length}`;
  $('playBtn').textContent = timer ? 'Ⅱ' : '▶';
  $('moveText').textContent = step === 0 ? `Starting at cylinder ${simulation.initial}` : `Cylinder ${simulation.moves[step - 1].from} → ${simulation.moves[step - 1].to}`;
  $('moveDistance').textContent = step === 0 ? '0' : simulation.moves[step - 1].distance;
  $('tableTotal').textContent = `Total: ${total} cylinders`;
  renderSequence(); renderTable(); updateChart(); drawDisk();
}
function renderSequence() {
  const nodes = [`<div class="seq-node"><span class="seq-chip ${step === 0 ? 'active' : 'done'}">${simulation.initial}</span></div>`];
  simulation.requests.forEach((n, i) => {
    nodes.push(`<span class="seq-arrow">→</span><div class="seq-node"><span class="seq-chip ${i < step ? 'done' : i === step ? 'active' : ''}">${n}</span></div>`);
  });
  $('sequence').innerHTML = nodes.join('');
}
function renderTable() {
  if (!simulation.moves.length) { $('movementTable').innerHTML = '<tr><td colspan="5" class="empty-cell">No requests to display.</td></tr>'; return; }
  $('movementTable').innerHTML = simulation.moves.map((m, i) => `<tr class="${i === step - 1 ? 'current-row' : ''}"><td>${String(m.index).padStart(2, '0')}</td><td>${m.from}</td><td>${m.to}</td><td>${m.distance}</td><td><span class="status-pill ${i < step ? 'complete' : ''}">${i < step ? 'SERVED' : 'PENDING'}</span></td></tr>`).join('');
}
function updateChart() {
  if (!simulation || !window.Chart) return;
  const labels = simulation.moves.map(m => `Req ${m.index}`);
  const distances = simulation.moves.map(m => m.distance);
  const colors = simulation.moves.map((m, i) => i < step ? '#57e3a1' : i === step ? '#55e6f0' : '#34445f');
  if (!chart) {
    chart = new Chart($('seekChart'), { type: 'bar', data: { labels, datasets: [{ label: 'Seek distance', data: distances, backgroundColor: colors, borderRadius: 6, borderSkipped: false, maxBarThickness: 34 }] }, options: { responsive: true, maintainAspectRatio: false, animation: { duration: 250 }, plugins: { legend: { display: false }, tooltip: { backgroundColor: '#111827', titleColor: '#edf4ff', bodyColor: '#55e6f0', padding: 12, displayColors: false } }, scales: { x: { grid: { display: false }, ticks: { color: '#8a9ab4', font: { family: 'DM Mono', size: 10 } }, border: { color: '#29364c' } }, y: { beginAtZero: true, grid: { color: '#ffffff0c' }, ticks: { color: '#8a9ab4', font: { family: 'DM Mono', size: 10 } }, border: { display: false } } } } });
  } else { chart.data.labels = labels; chart.data.datasets[0].data = distances; chart.data.datasets[0].backgroundColor = colors; chart.update(); }
}
function drawDisk() {
  const canvas = $('diskCanvas'); if (!canvas || !simulation) return;
  const rect = canvas.getBoundingClientRect(); if (!rect.width) return;
  const dpr = window.devicePixelRatio || 1; canvas.width = rect.width * dpr; canvas.height = rect.height * dpr;
  const ctx = canvas.getContext('2d'); ctx.scale(dpr, dpr);
  const w = rect.width, h = rect.height; ctx.clearRect(0, 0, w, h);
  const left = 28, right = w - 28, y = h * .54, span = right - left;
  const x = v => left + (v / (diskLimit - 1)) * span;
  // cylinder baseline and ticks
  ctx.strokeStyle = '#35465e'; ctx.lineWidth = 2; ctx.beginPath(); ctx.moveTo(left, y); ctx.lineTo(right, y); ctx.stroke();
  const ticks = 10;
  ctx.font = '10px DM Mono, monospace'; ctx.textAlign = 'center';
  for (let i = 0; i <= ticks; i++) {
    const val = Math.round((diskLimit - 1) * i / ticks), px = x(val);
    ctx.strokeStyle = '#42536c'; ctx.lineWidth = 1; ctx.beginPath(); ctx.moveTo(px, y - 7); ctx.lineTo(px, y + 8); ctx.stroke();
    ctx.fillStyle = '#7487a3'; ctx.fillText(val, px, y + 25);
  }
  // request dots; stack overlapping requests vertically
  const seen = {};
  simulation.positions.forEach((val, i) => {
    seen[val] = (seen[val] || 0) + 1;
    const offset = ((seen[val] - 1) % 3) * 13;
    const px = x(val), py = y - 23 - offset;
    if (i > 0 && i <= step) {
      const prev = simulation.positions[i - 1], pprev = x(prev);
      ctx.strokeStyle = '#57e3a1'; ctx.globalAlpha = .72; ctx.lineWidth = 2;
      ctx.beginPath(); ctx.moveTo(pprev, y - 23); ctx.lineTo(px, py); ctx.stroke(); ctx.globalAlpha = 1;
    }
    if (i === 0) return;
    const served = i <= step;
    ctx.beginPath(); ctx.arc(px, py, 5, 0, Math.PI * 2); ctx.fillStyle = served ? '#57e3a1' : '#62738f'; ctx.fill();
    ctx.strokeStyle = served ? '#57e3a1' : '#8d9cb2'; ctx.lineWidth = 1; ctx.stroke();
    ctx.fillStyle = served ? '#8cf4c4' : '#aebbd0'; ctx.font = '10px DM Mono, monospace'; ctx.fillText(val, px, py - 10);
  });
  // current head marker
  const hx = x(headPosition), hy = y + 1;
  ctx.shadowColor = '#55e6f0'; ctx.shadowBlur = 17; ctx.beginPath(); ctx.arc(hx, hy, 7, 0, Math.PI * 2); ctx.fillStyle = '#55e6f0'; ctx.fill(); ctx.shadowBlur = 0;
  ctx.strokeStyle = '#d8fdff'; ctx.lineWidth = 2; ctx.beginPath(); ctx.moveTo(hx, hy + 9); ctx.lineTo(hx, h - 19); ctx.stroke();
  ctx.fillStyle = '#55e6f0'; ctx.font = 'bold 10px DM Mono, monospace'; ctx.textAlign = hx > w - 70 ? 'right' : hx < 70 ? 'left' : 'center'; ctx.fillText('HEAD', hx, h - 7);
  // subtle position guide
  ctx.strokeStyle = '#55e6f044'; ctx.setLineDash([3, 5]); ctx.beginPath(); ctx.moveTo(hx, 13); ctx.lineTo(hx, y - 12); ctx.stroke(); ctx.setLineDash([]);
}
function nextStep() {
  if (!simulation || step >= simulation.requests.length) { stopPlayback(); return; }
  previousHead = simulation.positions[step]; step++; renderAll();
  if (step >= simulation.requests.length) stopPlayback();
}
function prevStep() { stopPlayback(); if (step > 0) step--; renderAll(); }
function stopPlayback() { if (timer) clearInterval(timer); timer = null; if ($('playBtn')) $('playBtn').textContent = '▶'; }
function startPlayback() {
  if (!simulation) buildSimulation(); if (!simulation || step >= simulation.requests.length) return;
  stopPlayback(); const delay = [1800, 1200, 750, 450, 220][Number($('speedInput').value) - 1];
  timer = setInterval(nextStep, delay); $('playBtn').textContent = 'Ⅱ';
}
function exportCSV() {
  if (!simulation) return;
  const rows = [['Step','From cylinder','To cylinder','Seek distance','Status']];
  simulation.moves.forEach((m, i) => rows.push([m.index, m.from, m.to, m.distance, i < step ? 'SERVED' : 'PENDING']));
  rows.push([], ['Total head movement', currentTotal()], ['Requests served', step]);
  const csv = rows.map(row => row.map(v => `"${String(v ?? '').replaceAll('"','""')}"`).join(',')).join('\r\n');
  const url = URL.createObjectURL(new Blob([csv], { type: 'text/csv;charset=utf-8;' })); const a = document.createElement('a'); a.href = url; a.download = 'fcfs-movement-report.csv'; a.click(); URL.revokeObjectURL(url);
}
$('runBtn').addEventListener('click', buildSimulation);
$('randomBtn').addEventListener('click', () => { diskLimit = Number($('diskSize').value); const count = 7; const vals = new Set(); while (vals.size < count) vals.add(Math.floor(Math.random() * diskLimit)); $('requestsInput').value = [...vals].join(', '); $('headInput').value = Math.floor(Math.random() * diskLimit); buildSimulation(); });
$('resetBtn').addEventListener('click', () => { stopPlayback(); step = 0; if (simulation) { $('requestsInput').value = simulation.requests.join(', '); $('headInput').value = simulation.initial; } buildSimulation(); });
$('exampleBtn').addEventListener('click', () => { $('requestsInput').value = EXAMPLE.join(', '); $('headInput').value = 50; $('diskSize').value = 200; buildSimulation(); });
$('playBtn').addEventListener('click', () => timer ? stopPlayback() : startPlayback());
$('nextBtn').addEventListener('click', () => { stopPlayback(); nextStep(); });
$('prevBtn').addEventListener('click', prevStep);
$('restartBtn').addEventListener('click', () => { stopPlayback(); step = 0; renderAll(); });
$('speedInput').addEventListener('input', () => { $('speedLabel').textContent = `${$('speedInput').value}×`; if (timer) startPlayback(); });
$('diskSize').addEventListener('change', () => { const max = Number($('diskSize').value) - 1; if (Number($('headInput').value) > max) $('headInput').value = max; });
$('downloadBtn').addEventListener('click', exportCSV);
$('themeBtn').addEventListener('click', () => { document.body.classList.toggle('light-mode'); drawDisk(); });
window.addEventListener('resize', drawDisk);
buildSimulation();