import axios from 'axios';

// API_URL is now relative. Vite 'proxy' will forward this to localhost:3001
const API_URL = '/api';

export const parkVehicle = async (plate, type, zone, area) => {
    try {
        const response = await axios.post(`${API_URL}/park`, { plate, type, zone, area });
        return response.data;
    } catch (error) {
        console.error("Park Error", error);
        return { result: 'error', message: 'Network error' };
    }
};

export const occupySlot = async (plate, type) => {
    try {
        const response = await axios.post(`${API_URL}/occupy`, { plate, type });
        return response.data;
    } catch (error) {
        console.error("Occupy Error", error);
        return { result: 'error', message: 'Network error' };
    }
};

export const releaseSlot = async (plate, type) => {
    try {
        const response = await axios.post(`${API_URL}/release`, { plate, type });
        return response.data;
    } catch (error) {
        console.error("Release Error", error);
        return { result: 'error', message: 'Network error' };
    }
};

export const getStatus = async () => {
    try {
        const response = await axios.get(`${API_URL}/status`);
        return response.data;
    } catch (error) {
        return { status: 'offline' };
    }
}

export const getHistory = async () => {
    try {
        const response = await axios.get(`${API_URL}/history`);
        return response.data.history || [];
    } catch (error) {
        return [];
    }
}
