/** @type {import('tailwindcss').Config} */
export default {
    content: [
        "./index.html",
        "./src/**/*.{js,ts,jsx,tsx}",
    ],
    theme: {
        extend: {
            colors: {
                'brand-dark': '#0f172a',
                'brand-panel': '#1e293b',
                'brand-accent': '#3b82f6',
                'slot-free': '#22c55e',
                'slot-occupied': '#ef4444',
                'slot-reserved': '#eab308',
            },
            fontFamily: {
                sans: ['Inter', 'sans-serif'],
            },
            animation: {
                'slide-in': 'slideIn 0.5s ease-out forwards',
            },
            keyframes: {
                slideIn: {
                    '0%': { transform: 'translateX(-100%)', opacity: '0' },
                    '100%': { transform: 'translateX(0)', opacity: '1' },
                }
            }
        },
    },
    plugins: [],
}
