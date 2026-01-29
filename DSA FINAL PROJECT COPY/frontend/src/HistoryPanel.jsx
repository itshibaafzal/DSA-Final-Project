import React, { useEffect, useState } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { getHistory } from './api';
import { Clock, Calendar, CheckCircle, XCircle } from 'lucide-react';

const HistoryPanel = () => {
    const [history, setHistory] = useState([]);

    useEffect(() => {
        const fetchHistory = async () => {
            const data = await getHistory();
            setHistory(data);
        };
        fetchHistory();
        const interval = setInterval(fetchHistory, 5000); // Update every 5s
        return () => clearInterval(interval);
    }, []);

    const formatTime = (epoch) => {
        if (!epoch || epoch === 0) return '-';
        return new Date(epoch * 1000).toLocaleTimeString();
    };

    const formatDuration = (hours) => {
        if (!hours) return '-';
        const h = Math.floor(hours);
        const m = Math.floor((hours - h) * 60);
        const s = Math.round(((hours - h) * 60 - m) * 60);
        return `${h}h ${m}m ${s}s`;
    };

    return (
        <div className="bg-[#1e293b] rounded-3xl border border-white/5 p-6 shadow-xl h-full flex flex-col">
            <h2 className="text-xl font-bold mb-4 flex items-center gap-2">
                <Clock className="text-blue-500" /> Recent Activity
            </h2>
            <div className="flex-1 overflow-y-auto custom-scrollbar space-y-3">
                <AnimatePresence>
                    {history.length === 0 ? <div className="text-slate-500 text-center mt-10">No history yet</div> :
                        history.map((item) => (
                            <motion.div
                                initial={{ opacity: 0, x: -20 }}
                                animate={{ opacity: 1, x: 0 }}
                                key={item.id}
                                className="bg-black/20 p-4 rounded-xl border border-white/5 hover:border-white/10 transition-colors"
                            >
                                <div className="flex justify-between items-start mb-2">
                                    <span className="font-mono font-bold text-lg">{item.vehicle}</span>
                                    <span className={`text-xs font-bold px-2 py-1 rounded ${item.status === 'RELEASED' ? 'bg-green-500/20 text-green-400' :
                                            item.status === 'OCCUPIED' ? 'bg-blue-500/20 text-blue-400' :
                                                'bg-slate-500/20 text-slate-400'
                                        }`}>
                                        {item.status}
                                    </span>
                                </div>

                                <div className="grid grid-cols-2 gap-y-2 text-xs text-slate-400">
                                    <div>Type: {item.type}</div>
                                    <div>Slot: {item.zone}-{item.area}-{item.slot}</div>

                                    <div>In: {formatTime(item.occupyTime || item.requestTime)}</div>
                                    <div>Out: {formatTime(item.releaseTime)}</div>
                                </div>

                                {item.durationHours > 0 && (
                                    <div className="mt-2 pt-2 border-t border-white/5 flex justify-between items-center">
                                        <span className="text-slate-500 text-[10px] uppercase font-bold">Duration</span>
                                        <span className="font-mono text-yellow-400 font-bold">{formatDuration(item.durationHours)}</span>
                                    </div>
                                )}
                            </motion.div>
                        ))}
                </AnimatePresence>
            </div>
        </div>
    );
};

export default HistoryPanel;
