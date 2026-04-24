# Orchidarc — static site

A no-build, fully static rebuild of orchidarc.org. Drop the whole folder onto
any static host (Cloudflare Pages, Netlify, GitHub Pages, Vercel) and you're live.

## File layout

    /                       <- the deployable site
    ├── index.html
    ├── films.html
    ├── cypripedium.html
    ├── acineta-barkeri.html
    ├── prosthechea-vitellina.html
    ├── extinct-strength.html
    ├── herbarium.html
    ├── gallery.html
    ├── stories.html
    ├── mexico-tours.html
    ├── about.html
    ├── take-action.html
    ├── store.html
    ├── socials.html
    ├── 404.html
    ├── style.css           <- shared styles (single source of truth)
    ├── app.js              <- shared JS (nav toggle, scroll reveal)
    ├── images/             <- put your photos here
    ├── videos/             <- put any self-hosted videos here
    └── build.py            <- regenerator (only needed if rebuilding pages)

## Adding images

Drop the file into `/images/`, then reference it with a relative path:

    <img src="images/cypripedium-population.jpg" alt="A short description">

Naming convention: lowercase, hyphens not spaces, descriptive.
Examples: `acineta-barkeri-flower.jpg`, `reserve-canopy-2024.jpg`.

**Optimise before uploading.** Run photos through https://squoosh.app or
https://tinypng.com. Targets:
  - Hero / full-bleed: ~1600px wide, 200–400 KB
  - Cards / thumbs:    ~800px wide,  100–200 KB
  - Format: `.jpg` for photos, `.webp` if you want smaller files,
    `.png` for graphics with transparency.

Always add an `alt` attribute — short description for screen readers and SEO.

## Adding videos

### Option A: YouTube (recommended for most cases)

You're already hosting on YouTube. Just embed:

    <div style="position:relative;padding-bottom:56.25%;height:0;overflow:hidden;border-radius:2px">
      <iframe
        src="https://www.youtube.com/embed/9ozJdv3ggG0"
        style="position:absolute;top:0;left:0;width:100%;height:100%"
        frameborder="0"
        allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
        allowfullscreen
        title="Lily of Allsaints"
      ></iframe>
    </div>

The 56.25% padding gives you a perfect 16:9 responsive embed.

### Option B: Self-hosted video (silent loops, hero reels)

For background loops or short reels, drop a small `.mp4` into `/videos/`:

    <video autoplay muted loop playsinline preload="auto"
           style="width:100%;height:100%;object-fit:cover">
      <source src="videos/canopy-loop.mp4" type="video/mp4">
    </video>

Keep it under 10 MB. Use Handbrake or `ffmpeg` to compress:

    ffmpeg -i input.mov -vcodec libx264 -crf 28 -preset slow \
           -an -movflags +faststart output.mp4

(`-an` strips audio — important for autoplay loops in browsers.)

## Editing content

Each `.html` file is **fully self-contained**. Open it in any text editor (or
Cursor / VS Code), change the text, save, refresh your browser. No build step.

## Editing the nav, footer, or styles

These appear on every page:

  - **Nav and footer:** the easiest way to keep them consistent across pages
    is to edit `build.py`, then run `python build.py` to regenerate every
    page at once. Otherwise, just find-and-replace across the `.html` files.
  - **Styles:** edit `style.css`. Changes apply everywhere automatically.
  - **JS:** edit `app.js`.

To regenerate from `build.py`:

    cd /path/to/site
    python build.py

(Python 3 only, no dependencies.)

## Subscribe form

Currently a placeholder — emails go nowhere. To wire it up, edit `app.js` and
plug in a real provider. Recommended:

  - **Buttondown** (~$9/mo, no ads, no creepy tracking) —
    https://buttondown.com — replace the form-submit handler with a `fetch()`
    POST to their embed endpoint.
  - **MailerLite** has a free tier up to 1,000 subscribers.
  - **Mailchimp** if you want the most popular option.

## Deploying

### Cloudflare Pages (free, fastest)

1. Drag this folder onto https://pages.cloudflare.com (or push to GitHub
   and connect the repo).
2. In your registrar, point `orchidarc.org` to the Cloudflare nameservers.
3. Cancel Squarespace.

### Netlify Drop (zero-config)

1. Go to https://app.netlify.com/drop
2. Drag the folder.
3. Update DNS at your registrar to point `orchidarc.org` at Netlify.

### GitHub Pages

1. Push this folder to a GitHub repo.
2. Settings → Pages → Source: `main` branch, `/ (root)`.
3. Add a `CNAME` file containing `orchidarc.org`.
4. Update DNS.

## Notes on the current site

- Subpages still link to **Squarespace CDN images** (so nothing breaks today).
  Replace those URLs with `images/whatever.jpg` paths as you migrate photos
  into `/images/`.
- The store page has no checkout — the original was Squarespace Commerce.
  When ready, plug in **Shopify Lite** ($5/mo, embed buttons), **Stripe
  Payment Links** (free, simplest for a small NGO), or **Big Cartel**.
- The donate flow on `take-action.html` is a placeholder. Add a real
  donation processor: GoCardless, Stripe, or a UK-charity-specific one
  like CAF or JustGiving.

— Built with care.
