// Orchidarc — shared front-end behaviour

// Mobile nav
const t = document.getElementById('navtoggle');
const l = document.getElementById('navlinks');
if (t && l) {
  t.addEventListener('click', () => l.classList.toggle('open'));
  l.querySelectorAll('a').forEach(a => a.addEventListener('click', () => l.classList.remove('open')));
}

// Reveal-on-scroll
const io = new IntersectionObserver(
  (entries) => entries.forEach(e => e.isIntersecting && e.target.classList.add('in')),
  { threshold: 0.12, rootMargin: '0px 0px -10% 0px' }
);
document.querySelectorAll('.reveal').forEach(el => io.observe(el));

// Subscribe form
// To wire to a real provider, replace the body of the submit handler with a fetch().
// Buttondown example:
//   fetch('https://buttondown.com/api/emails/embed-subscribe/orchidarc', { method:'POST', body: new FormData(f) })
const f = document.getElementById('subform');
const m = document.getElementById('submsg');
const e = document.getElementById('subemail');
if (f) {
  f.addEventListener('submit', (ev) => {
    ev.preventDefault();
    const v = (e.value || '').trim();
    if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(v)) {
      m.textContent = '— Please enter a valid email';
      return;
    }
    m.textContent = "— Thank you. We'll be in touch from the cloud forest.";
    e.value = '';
  });
}
