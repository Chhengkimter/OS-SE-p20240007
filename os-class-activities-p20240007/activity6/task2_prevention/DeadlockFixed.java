import java.util.concurrent.Semaphore;

class AccountFixed {
    String name;
    int balance;

    AccountFixed(String name, int balance) {
        this.name = name;
        this.balance = balance;
    }
}

class SafeTransfer {
    static Semaphore mutex = new Semaphore(1);

    static void transfer(AccountFixed from, AccountFixed to, int amount) {
        try {
            System.out.println(Thread.currentThread().getName()
                    + " waiting for mutex...");
            mutex.acquire();
            System.out.println(Thread.currentThread().getName()
                    + " acquired mutex, starting transfer: "
                    + amount + " from " + from.name + " to " + to.name);

            Thread.sleep(100);

            from.balance -= amount;
            to.balance += amount;

            System.out.println(Thread.currentThread().getName()
                    + " transferred " + amount + " from " + from.name + " to " + to.name
                    + " | " + from.name + ": " + from.balance
                    + " | " + to.name + ": " + to.balance);
        } catch (InterruptedException e) {
            System.out.println(Thread.currentThread().getName() + " interrupted.");
        } finally {
            mutex.release();
            System.out.println(Thread.currentThread().getName() + " released mutex.");
        }
    }
}

public class DeadlockFixed {
    public static void main(String[] args) throws InterruptedException {
        AccountFixed accountA = new AccountFixed("Account-A", 1000);
        AccountFixed accountB = new AccountFixed("Account-B", 1000);

        System.out.println("=== Bank Transfer Deadlock Prevention ===");
        System.out.println("Starting balances -> Account-A: " + accountA.balance
                + " | Account-B: " + accountB.balance);
        int startTotal = accountA.balance + accountB.balance;
        System.out.println("Starting total: " + startTotal);
        System.out.println();

        Thread t1 = new Thread(() ->
                SafeTransfer.transfer(accountA, accountB, 100), "Thread-1");

        Thread t2 = new Thread(() ->
                SafeTransfer.transfer(accountB, accountA, 200), "Thread-2");

        t1.start();
        t2.start();
        t1.join();
        t2.join();

        int finalTotal = accountA.balance + accountB.balance;
        System.out.println();
        System.out.println("=== Final Results ===");
        System.out.println("Final Account-A: " + accountA.balance);
        System.out.println("Final Account-B: " + accountB.balance);
        System.out.println("Final total: " + finalTotal);
        System.out.println("Starting total: " + startTotal + " | Matches: " + (startTotal == finalTotal));
        System.out.println();
        System.out.println("No deadlock occurred");
    }
}
