package org.defix.eventSystem.abstractions;

import java.util.concurrent.CopyOnWriteArrayList;
import java.util.function.Function;

public class UnaryEvent<T> {
    private final CopyOnWriteArrayList<Function<T, Void>> listeners = new CopyOnWriteArrayList<>();

    public void addListener(Function<T, Void> listener) {
        listeners.add(listener);
    }

    public void removeListener(Function<T, Void> listener) {
        listeners.remove(listener);
    }

    public void invoke(T value) {
        for(Function<T, Void> listener : listeners) {
            listener.apply(value);
        }
    }
}
