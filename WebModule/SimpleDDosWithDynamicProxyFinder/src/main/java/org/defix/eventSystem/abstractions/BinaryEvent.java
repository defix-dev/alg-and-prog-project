package org.defix.eventSystem.abstractions;

import java.util.concurrent.CopyOnWriteArrayList;
import java.util.function.BiFunction;

public class BinaryEvent<T1, T2> {
    private final CopyOnWriteArrayList<BiFunction<T1, T2, Void>> listeners = new CopyOnWriteArrayList<>();

    public void addListener(BiFunction<T1, T2, Void> listener) {
        listeners.add(listener);
    }

    public void removeListener(BiFunction<T1, T2, Void> listener) {
        listeners.remove(listener);
    }

    public void invoke(T1 value1, T2 value2) {
        for(BiFunction<T1, T2, Void> listener : listeners) {
            listener.apply(value1, value2);
        }
    }
}

