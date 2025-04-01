package org.defix;

public class MyBinaryTree {
    private static class Node {
        public Node left;
        public Node right;
        public int value;

        public Node(int value) {
            this.value = value;
        }
    }

    private Node root;

    public void insert(int value) {
        root = insertRec(root, value);
    }

    private Node insertRec(Node node, int value) {
        if(node == null) return new Node(value);
        if(value < node.value) node.left = insertRec(node.left, value);
        else if(value > node.value) node.right = insertRec(node.right, value);
        return node;
    }

    public boolean search(int value) {
        return searchRec(root, value);
    }

    private boolean searchRec(Node node, int value) {
        if(node == null) return false;
        if(node.value == value) return true;
        return value < node.value ? searchRec(node.left, value) : searchRec(node.right, value);
    }

    public void delete(int value) {
        root = deleteRec(root, value);
    }

    private Node deleteRec(Node node, int value) {
        if(node == null) return null;
        if(value < node.value) node.left = deleteRec(node.left, value);
        else if(value > node.value) node.right = deleteRec(node.right, value);
        else {
            if(node.left == null && node.right == null) return null;
            if(node.left == null) return node.right;
            if(node.right == null) return node.left;
            node.value = getMinValue(node.right);
            root.right = deleteRec(node.right, node.value);
        }
        return node;
    }

    private int getMinValue(Node node) {
        int minValue = node.value;
        while(node.left != null) {
            node = node.left;
            minValue = node.value;
        }
        return minValue;
    }
}
