import React, { useState, useEffect } from 'react';
import { Truck, Car, Bike, Activity, Zap, RefreshCw } from 'lucide-react';
import { motion, AnimatePresence } from 'framer-motion';
import { parkVehicle, occupySlot, releaseSlot, getStatus } from './api';
import HistoryPanel from './HistoryPanel';

function App() {
  const [zones, setZones] = useState([]);
  const [selectedZoneId, setSelectedZoneId] = useState(1);
  const [mode, setMode] = useState('park');
  const [formData, setFormData] = useState({ plate: '', type: '1', zone: '1', area: '0' });
  const [logs, setLogs] = useState([]);
  const [loading, setLoading] = useState(false);
  const [isLive, setIsLive] = useState(false);
  const [viewHistory, setViewHistory] = useState(false);

  // Poll payload
  useEffect(() => {
    let interval;
    const fetchData = async () => {
      try {
        const res = await getStatus();
        if (res.status === 'running') {
          setIsLive(true);
        } else if (res.zones) {
          setZones(res.zones);
          setIsLive(true);
        }
      } catch (e) {
        setIsLive(false);
      }
    };

    fetchData(); // Initial
    interval = setInterval(fetchData, 2000); // Poll every 2s
    return () => clearInterval(interval);
  }, []);

  const addLog = (msg, type = 'info') => {
    setLogs(prev => [{ id: Date.now(), msg, type }, ...prev].slice(0, 50));
  };

  const handleAction = async (e) => {
    e.preventDefault();
    setLoading(true);

    try {
      if (mode === 'park') {
        const res = await parkVehicle(formData.plate, parseInt(formData.type), parseInt(formData.zone), parseInt(formData.area));
        if (res.result === 'success') addLog(`✅ Success! Vehicle ${formData.plate} is allocated.`, 'success');
        else addLog(`❌ Oops! ${res.message}`, 'error');
      }
      else if (mode === 'occupy') {
        const res = await occupySlot(formData.plate, parseInt(formData.type));
        if (res.result === 'success') addLog(`🚗 Vehicle ${formData.plate} has arrived.`, 'success');
        else addLog('Could not occupy slot. Check details.', 'error');
      }
      else if (mode === 'release') {
        const res = await releaseSlot(formData.plate, parseInt(formData.type));
        if (res.result === 'success') addLog(`👋 Bye! Vehicle ${formData.plate} released.`, 'success');
        else addLog('Release failed. Is it parked?', 'error');
      }
    } catch (err) { addLog('System Error', 'error'); }
    setLoading(false);
  };

  const selectedZone = zones.find(z => z.id === selectedZoneId);

  return (
    <div className="min-h-screen bg-[#0f172a] text-white p-6 flex gap-6 font-sans overflow-hidden">

      {/* LEFT PANEL */}
      <motion.div
        initial={{ x: -100, opacity: 0 }}
        animate={{ x: 0, opacity: 1 }}
        className="w-[400px] flex flex-col gap-6"
      >
        {/* Header */}
        <div className="p-6 rounded-3xl bg-[#1e293b]/50 backdrop-blur-xl border border-white/10 shadow-2xl relative overflow-hidden group">
          <div className="absolute inset-0 bg-gradient-to-r from-blue-500/10 to-purple-500/10 opacity-0 group-hover:opacity-100 transition-opacity duration-500"></div>
          <h1 className="text-4xl font-black italic bg-gradient-to-br from-white to-slate-500 bg-clip-text text-transparent">
            SMART<span className="text-blue-500">PARK</span>
          </h1>
          <p className="text-slate-400 text-sm mt-1">Zone Management System</p>
          <div className="flex items-center gap-2 mt-2">
            <div className={`w-2 h-2 rounded-full ${isLive ? 'bg-green-500 animate-pulse' : 'bg-red-500'}`}></div>
            <span className="text-xs font-mono text-slate-400">{isLive ? 'SYSTEM ONLINE' : 'CONNECTING...'}</span>
          </div>
        </div>

        {/* Controls */}
        <div className="p-6 rounded-3xl bg-[#1e293b] border border-white/5 shadow-xl flex-1 flex flex-col">
          {/* Tabs */}
          <div className="flex p-1 bg-black/20 rounded-xl mb-6">
            <button onClick={() => setMode('park')} className={`flex-1 py-3 rounded-lg text-sm font-bold transition-all ${mode === 'park' ? 'bg-blue-600 text-white' : 'text-slate-500'}`}>PARK</button>
            <button onClick={() => setMode('occupy')} className={`flex-1 py-3 rounded-lg text-sm font-bold transition-all ${mode === 'occupy' ? 'bg-emerald-600 text-white' : 'text-slate-500'}`}>ARRIVE</button>
            <button onClick={() => setMode('release')} className={`flex-1 py-3 rounded-lg text-sm font-bold transition-all ${mode === 'release' ? 'bg-rose-600 text-white' : 'text-slate-500'}`}>LEAVE</button>
          </div>

          <form onSubmit={handleAction} className="space-y-5">
            <div className="space-y-2">
              <label className="text-xs font-bold text-slate-500 uppercase ml-1">Vehicle Details</label>
              <input
                value={formData.plate}
                onChange={e => setFormData({ ...formData, plate: e.target.value.toUpperCase() })}
                placeholder="ENTER PLATE NO."
                className="w-full bg-slate-900/50 border border-white/10 rounded-xl p-4 font-mono text-lg focus:border-blue-500 focus:ring-1 focus:ring-blue-500 outline-none transition-all placeholder:text-slate-600"
              />
            </div>

            <div className="grid grid-cols-2 gap-4">
              <div
                onClick={() => setFormData({ ...formData, type: '1' })}
                className={`p-4 rounded-xl border-2 cursor-pointer transition-all flex flex-col items-center gap-2 ${formData.type === '1' ? 'border-blue-500 bg-blue-500/10' : 'border-slate-700 hover:border-slate-500'
                  }`}
              >
                <Car /> <span className="text-xs font-bold">CAR</span>
              </div>
              <div
                onClick={() => setFormData({ ...formData, type: '2' })}
                className={`p-4 rounded-xl border-2 cursor-pointer transition-all flex flex-col items-center gap-2 ${formData.type === '2' ? 'border-blue-500 bg-blue-500/10' : 'border-slate-700 hover:border-slate-500'
                  }`}
              >
                <Bike /> <span className="text-xs font-bold">BIKE</span>
              </div>
            </div>

            {mode === 'park' && (
              <div className="space-y-2">
                <label className="text-xs font-bold text-slate-500 uppercase ml-1">Location Preference</label>
                <div className="flex gap-4">
                  <select
                    value={formData.zone}
                    onChange={e => setFormData({ ...formData, zone: e.target.value })}
                    className="flex-1 bg-slate-900/50 border border-white/10 rounded-xl p-4 outline-none text-sm"
                  >
                    {Array.from({ length: 15 }, (_, i) => i + 1).map(z => <option key={z} value={z}>Zone {z}</option>)}
                  </select>
                  <select
                    value={formData.area}
                    onChange={e => setFormData({ ...formData, area: e.target.value })}
                    className="flex-1 bg-slate-900/50 border border-white/10 rounded-xl p-4 outline-none text-sm"
                  >
                    <option value="0">Any Area</option>
                    <option value="1">Area 1</option>
                    <option value="2">Area 2</option>
                    <option value="3">Area 3</option>
                  </select>
                </div>
              </div>
            )}

            <button
              disabled={loading}
              className={`w-full py-4 rounded-xl font-bold text-lg shadow-lg active:scale-95 transition-transform ${mode === 'park' ? 'bg-gradient-to-r from-blue-600 to-indigo-600 shadow-blue-900/20' :
                  mode === 'occupy' ? 'bg-gradient-to-r from-emerald-600 to-teal-600 shadow-emerald-900/20' :
                    'bg-gradient-to-r from-rose-600 to-pink-600 shadow-rose-900/20'
                }`}
            >
              {loading ? 'PROCESSING...' : mode === 'park' ? 'ALLOCATE SPOT' : mode === 'occupy' ? 'CONFIRM ARRIVAL' : 'RELEASE VEHICLE'}
            </button>
          </form>

          {/* Logs */}
          <div className="mt-6 flex-1 overflow-hidden flex flex-col min-h-[100px]">
            <div className="flex-1 overflow-y-auto space-y-2 pr-2 custom-scrollbar">
              <AnimatePresence initial={false}>
                {logs.map((log) => (
                  <motion.div
                    key={log.id}
                    initial={{ opacity: 0, x: -20 }}
                    animate={{ opacity: 1, x: 0 }}
                    exit={{ opacity: 0 }}
                    className={`text-xs p-3 rounded-lg border-l-2 ${log.type === 'success' ? 'border-green-500 bg-green-500/10 text-green-300' :
                        log.type === 'error' ? 'border-red-500 bg-red-500/10 text-red-300' :
                          'border-slate-500 bg-slate-500/10 text-slate-300'
                      }`}
                  >
                    {log.msg}
                  </motion.div>
                ))}
              </AnimatePresence>
            </div>
          </div>

        </div>
      </motion.div>

      {/* RIGHT PANEL - MONITOR */}
      <div className="flex-1 flex flex-col gap-6 overflow-hidden">
        {/* Zone Selector */}
        <div className="flex justify-between items-center bg-[#1e293b] p-2 rounded-2xl border border-white/5">
          <div className="h-12 flex items-center gap-2 overflow-x-auto custom-scrollbar flex-1 mr-4">
            {zones.length > 0 ? zones.map(z => (
              <button
                key={z.id}
                onClick={() => setSelectedZoneId(z.id)}
                className={`h-10 w-10 shrink-0 rounded-lg font-bold flex items-center justify-center transition-all ${selectedZoneId === z.id
                    ? 'bg-blue-600 text-white shadow-lg shadow-blue-500/30'
                    : 'bg-black/20 text-slate-500 hover:text-white border border-white/5'
                  }`}
              >
                {z.id}
              </button>
            )) : <div className="text-slate-500 animate-pulse px-4">Loading Zones...</div>}
          </div>

          <button
            onClick={() => setViewHistory(!viewHistory)}
            className={`flex items-center gap-2 px-4 py-2 rounded-xl font-bold text-xs border transition-all ${viewHistory ? 'bg-blue-600 border-blue-500 text-white' : 'bg-black/20 border-white/10 text-slate-400 hover:text-white'
              }`}
          >
            <Activity size={16} /> {viewHistory ? 'CLOSE HISTORY' : 'HISTORY'}
          </button>
        </div>

        {/* Content Area */}
        {viewHistory ? (
          <div className="flex-1 overflow-hidden">
            <HistoryPanel />
          </div>
        ) : (
          <div className="flex-1 bg-[#1e293b] rounded-3xl border border-white/5 p-8 overflow-y-auto shadow-2xl relative">
            {!selectedZone && <div className="text-center text-slate-500 mt-20">Select a Zone to View</div>}

            {selectedZone && (
              <>
                <div className="flex justify-between items-end mb-6 border-b border-white/5 pb-4">
                  <div>
                    <h2 className="text-3xl font-bold text-white tracking-tight">ZONE {selectedZone.id}</h2>
                    <p className="text-slate-400 text-sm">{selectedZone.name || 'General Parking Zone'}</p>
                  </div>
                  <div className="text-right">
                    <div className="text-xl font-mono text-green-400">{selectedZone.areas.reduce((acc, a) => acc + a.slots.filter(s => s.isAvailable).length, 0)} SPLOTS FREE</div>
                  </div>
                </div>

                <div className="grid grid-cols-1 xl:grid-cols-3 gap-8">
                  {selectedZone.areas.map(area => (
                    <motion.div
                      initial={{ opacity: 0, y: 20 }}
                      animate={{ opacity: 1, y: 0 }}
                      key={area.id}
                      className="bg-black/20 rounded-2xl p-4 border border-white/5"
                    >
                      <h3 className="text-slate-400 font-mono text-sm mb-4 flex justify-between">
                        {area.name}
                        <span className="text-slate-600 text-xs">{area.slots.filter(s => s.isAvailable).length} FREE</span>
                      </h3>
                      <div className="grid grid-cols-4 gap-3">
                        {area.slots.map(slot => (
                          <div
                            key={slot.id}
                            className={`aspect-square rounded-lg flex items-center justify-center text-xs font-mono transition-all duration-500 ${slot.isAvailable
                                ? 'bg-[#0f172a] border border-slate-700/50 text-slate-600'
                                : 'bg-gradient-to-br from-red-500 to-pink-600 shadow-lg shadow-red-500/20 text-white border-transparent scale-105'
                              }`}
                          >
                            {slot.id % 20 === 0 ? 20 : slot.id % 20}
                          </div>
                        ))}
                      </div>
                    </motion.div>
                  ))}
                </div>
              </>
            )}
          </div>
        )}

        {/* Legend */}
        {!viewHistory && (
          <div className="flex justify-center gap-8 text-xs font-bold text-slate-500 uppercase tracking-widest">
            <div className="flex items-center gap-2"><div className="w-3 h-3 rounded bg-[#0f172a] border border-slate-700"></div> Available</div>
            <div className="flex items-center gap-2"><div className="w-3 h-3 rounded bg-gradient-to-br from-red-500 to-pink-600"></div> Occupied</div>
          </div>
        )}
      </div>

    </div>
  );
}

export default App;
